#pragma once

#include <iterator>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <memory>

#include "core.h"
#include "util/greedy_vector.h"
#include "math/vector.h"

#include "graphics/context.h"
#include "graphics/texture.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"
#include "graphics/shader.h"
#include "tex_coords.h"

#define OGL_2D_BATCH_MAX_SPRITES 10'000
#define OGL_2D_BATCH_MAX_VERTS OGL_2D_BATCH_MAX_SPRITES * 4
#define OGL_2D_BATCH_MAX_INDICES OGL_2D_BATCH_MAX_SPRITES * 6

namespace ogl {

	// Struct of arrays
	struct soa {};
	// Array of structures
	struct aos {};

	template<typename PosIter,
			 typename SizeIter,
			 typename ColIter,
			 typename TCIter,
			 typename TexIter,
			 typename DataFormat = aos>
	struct Renderer2DSpriteData {

		using data_format = DataFormat;

		static_assert(std::is_same_v<std::remove_const_t<typename std::iterator_traits<PosIter>::value_type>, Vector3f>);
		static_assert(std::is_same_v<std::remove_const_t<typename std::iterator_traits<SizeIter>::value_type>, Vector2f>);
		static_assert(std::is_same_v<std::remove_const_t<typename std::iterator_traits<ColIter>::value_type>, Vector4f>);
		static_assert(std::is_same_v<std::remove_const_t<typename std::iterator_traits<TCIter>::value_type>, TexCoords>);
		static_assert(std::is_same_v<std::remove_const_t<typename std::iterator_traits<TexIter>::value_type>, Texture2D>);

		Renderer2DSpriteData(PosIter p, SizeIter s, ColIter c, TCIter tc, TexIter ti, size_t count) 
			: pos(p), size(s), col(c), texCoords(tc), texture(ti), spriteCount(count) {}

		Renderer2DSpriteData offset(size_t count) const {
			OGL_DEBUG_ASSERT(count <= spriteCount);
			return Renderer2DSpriteData(pos + count, size + count, col + count, 
					texCoords + count, texture + count, spriteCount - count);
		}

		Renderer2DSpriteData subset(size_t offset, size_t count) const {
			auto data = this->offset(offset);
			OGL_DEBUG_ASSERT(count <= data.spriteCount);
			data.spriteCount = count;
			return data;
		}

		Renderer2DSpriteData operator++(int) {
			auto ret = *this;
			pos++; size++; col++; texCoords++; texture++;
			return ret;
		}

		PosIter pos;
		SizeIter size;
		ColIter col;
		TCIter texCoords;
		TexIter texture;

		size_t spriteCount;
	};

	class Renderer2D {
		public:

			struct SpriteVertex {
				Vector3f position;
				Vector4f colour;
				Vector2f texCoord;
				texslot_t texId;
			};

			Renderer2D(const GraphicsContext& context);
			Renderer2D(const Renderer2D&) = delete;
			Renderer2D(Renderer2D&&) = delete;
			~Renderer2D();
			
			template<typename A, typename B, typename C, typename D, typename E>
			void process_dynamic(const Renderer2DSpriteData<A, B, C, D, E>&, const GraphicsContext&);
			

			// Always make sure this is called before the end of each frame
			// To make sure no buffered data is lying around
			void flush(const GraphicsContext&);

			float get_near() { return m_Near; }
			void set_near(float n) { m_Near = n; }

			float get_far() { return m_Far; }
			void set_far(float f) { m_Far = f; }
		private:
			template<typename A, typename B, typename C, typename D, typename E>
			void transform_data(const Renderer2DSpriteData<A, B, C, D, E>&);
			Matrix4f calc_ortho_mat(const GraphicsContext&);
		private:
			VertexBuffer m_BatchVBO;
			IndexBuffer m_BatchIBO;
			VertexArray m_VAO;
			std::unique_ptr<Shader> m_Shader;

			MemView<SpriteVertex> m_BatchMappedVBO;
			size_t m_BatchSpriteCount = 0;
			std::unordered_map<uint32_t, texslot_t> m_BatchTexIds; // Maps a textures gl id to a texture slot.
			const int m_MaxTextureSlots;

			float m_Near = -1.0f;
			float m_Far = 100.0f;
	};

	template<typename A, typename B, typename C, typename D, typename E>
	void Renderer2D::process_dynamic(const Renderer2DSpriteData<A, B, C, D, E>& data, const GraphicsContext& context) {
	
		//TODO: change to not require random access iterator (only really need foward iterator)

		// Because texture slots are the limiting factor
		// we need to make sure the batches are limited 
		// by texture slots, and also sprite count.

		// This will hold from where we need to transform the last set of data
		size_t dataOffsetIter = ;
		for(size_t i = 0; i < data.spriteCount; i++) {

			if(m_BatchSpriteCount + i == OGL_2D_BATCH_MAX_SPRITES) {
				// If we are limited by batch size we process and flush  
				auto renderData = data.subset(dataOffset, i - dataOffset);
				transform_data(renderData);
				flush(context);
				dataOffset = i;
			}

			// Check to see if we are at our texture limit
			auto &tex = *(data.texture + i);
			const auto rid = tex.get_renderer_id();
			const auto alreadyExistsIter = m_BatchTexIds.find(rid);

			if(alreadyExistsIter == m_BatchTexIds.end()) {
				// We have a new texture	
				texslot_t id = m_BatchTexIds.size();
				tex.set_texid(id);
				m_BatchTexIds[rid] = id;

				if(m_BatchTexIds.size() == m_MaxTextureSlots) {		
					// The batch is now full, so we need to flush after processing 
					// this batch.
					auto renderData = data.subset(dataOffset, i - dataOffset);
					transform_data(renderData);
					flush(context);
					dataOffset = i;
				}
			}
		}

		auto renderData = data.offset(dataOffset);
		transform_data(renderData);
	}

	// TODO: add specialisation for soa vs. aos
	template<typename A, typename B,typename C, typename D, typename E>
	void Renderer2D::transform_data(const Renderer2DSpriteData<A,B,C,D,E>& data) {

		size_t startIndex = m_BatchSpriteCount * 4;

		// TODO: Make this more DOD friendly?
		size_t vertIndex = startIndex;
		auto posIter = data.pos;
		auto sizeIter = data.size;
		for(size_t i = 0; i < data.spriteCount; i++) {
			const auto& pos = *posIter++;
			const auto& size = *sizeIter++;
			m_BatchMappedVBO[vertIndex + 0].position = pos;
			m_BatchMappedVBO[vertIndex + 1].position = Vector3f{ pos.x, pos.y + size.y, pos.z };
			m_BatchMappedVBO[vertIndex + 2].position = Vector3f{ pos.x + size.x, pos.y + size.y, pos.z};
			m_BatchMappedVBO[vertIndex + 3].position = Vector3f{ pos.x + size.x, pos.y, pos.z};
			vertIndex += 4;
		}

		vertIndex = startIndex;
		auto colIter = data.col;
		for(size_t i = 0; i < data.spriteCount; i++) {
			const auto& col = *colIter++;
			m_BatchMappedVBO[vertIndex + 0].colour = col;
			m_BatchMappedVBO[vertIndex + 1].colour = col;
			m_BatchMappedVBO[vertIndex + 2].colour = col;
			m_BatchMappedVBO[vertIndex + 3].colour = col;
			vertIndex += 4;
		}

		vertIndex = startIndex;
		auto tcIter = data.texCoords;
		for(size_t i = 0; i < data.spriteCount; i++) {
			const auto& tc = *tcIter++;
			m_BatchMappedVBO[vertIndex + 0].texCoord = tc.pos;
			m_BatchMappedVBO[vertIndex + 1].texCoord = tc.pos + Vector2f{0.0f, tc.size.y};
			m_BatchMappedVBO[vertIndex + 2].texCoord = tc.pos + tc.size;
			m_BatchMappedVBO[vertIndex + 3].texCoord = tc.pos + Vector2f{tc.size.x, 0.0f};
			vertIndex += 4;
		}

		vertIndex = startIndex;
		auto texIter = data.texture;
		for(size_t i = 0; i < data.spriteCount; i++) {
			const auto& rid = (*texIter++).get_renderer_id();
			const auto& texId = m_BatchTexIds[rid];
			m_BatchMappedVBO[vertIndex + 0].texId = texId;
			m_BatchMappedVBO[vertIndex + 1].texId = texId;
			m_BatchMappedVBO[vertIndex + 2].texId = texId;
			m_BatchMappedVBO[vertIndex + 3].texId = texId;
			vertIndex += 4;
		}

		m_BatchSpriteCount += data.spriteCount;
	}
}
