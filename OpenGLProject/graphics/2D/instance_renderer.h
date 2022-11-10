#pragma once

#include "core.h"
#include "util/greedy_vector.h"
#include "math/vector.h"

#include "graphics/context.h"
#include "graphics/texture.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"
#include "graphics/shader.h"
#include "tex_coords.h"
#include "sprite_data.h"

#define OGL_2D_BATCH_MAX_SPRITES 10'000
#define OGL_2D_BATCH_MAX_VERTS OGL_2D_BATCH_MAX_SPRITES * 4
#define OGL_2D_BATCH_MAX_INDICES OGL_2D_BATCH_MAX_SPRITES * 6

namespace ogl {

	class BatchRenderer2D {
		public:

			struct SpriteVertex {
				Vector3f position;
				Vector4f colour;
				Vector2f texCoord;
				texslot_t texId;
			};

			BatchRenderer2D(const GraphicsContext& context);
			BatchRenderer2D(const BatchRenderer2D&) = delete;
			BatchRenderer2D(BatchRenderer2D&&) = delete;
			~BatchRenderer2D();
		
			// process takes a RendererSpriteData struct and processes it for
			// rendering. With an instanced renderer the sprite data should be sorted by
			// texture to maxmise efficiency.
			void process(const RendererSpriteData&, const GraphicsContext&);

			// Always make sure this is called before the end of each frame
			// To make sure no buffered data is lying around
			void flush(const GraphicsContext&);

			float get_near() { return m_Near; }
			void set_near(float n) { m_Near = n; }

			float get_far() { return m_Far; }
			void set_far(float f) { m_Far = f; }

		private:
			void transform_data(const RendererSpriteData& data);
			Matrix4f calc_ortho_mat(const GraphicsContext& ctx);

			VertexBuffer m_BatchVBO;
			IndexBuffer m_BatchIBO;
			VertexArray m_VAO;
			std::unique_ptr<Shader> m_Shader;

			MemView<SpriteVertex> m_BatchMappedVBO;
			size_t m_BatchSpriteCount = 0;
			int32_t m_CurrentTexSlot = 0;
			const int m_MaxTextureSlots;

			float m_Near = -1.0f;
			float m_Far = 100.0f;
	};
}
