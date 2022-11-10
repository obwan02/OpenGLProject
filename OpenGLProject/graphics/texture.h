#pragma once
#include <glad/glad.h>
#include "util/image.h"

namespace ogl {

	enum class FilterMode : int32_t {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	enum class WrapMode : int32_t {
		ClampToBorder = GL_CLAMP_TO_BORDER,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT
	};

	using texslot_t = uint32_t;
	inline constexpr texslot_t unassigned_texid = -1U;

	struct TextureAssetParams {
		bool generateMipMaps;
		FilterMode mipMapFilterMode;
		FilterMode filterMode;
		WrapMode wrapMode;
	};

	class Texture2D {
	public:
		Texture2D(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other) : m_GlId(other.m_GlId) { other.m_GlId = 0; }
		~Texture2D() { if(m_GlId) glDeleteTextures(1, &m_GlId); }

		Texture2D(const Image& image, 
					bool generateMipMaps = true,
					FilterMode mipMapFilterMode = FilterMode::Linear,
					FilterMode filterMode = FilterMode::Linear, 
					WrapMode wrapMode = WrapMode::ClampToBorder) {
			glGenTextures(1, &m_GlId);
			glBindTexture(GL_TEXTURE_2D, m_GlId);
			
			int32_t minFilterMode;
			if (generateMipMaps) {
				if (mipMapFilterMode == FilterMode::Nearest && filterMode == FilterMode::Nearest)
					minFilterMode = GL_NEAREST_MIPMAP_NEAREST;
				else if (mipMapFilterMode == FilterMode::Nearest && filterMode == FilterMode::Linear)
					minFilterMode = GL_LINEAR_MIPMAP_NEAREST;
				else if (mipMapFilterMode == FilterMode::Linear && filterMode == FilterMode::Nearest)
					minFilterMode = GL_NEAREST_MIPMAP_LINEAR;
				else if (mipMapFilterMode == FilterMode::Linear && filterMode == FilterMode::Linear)
					minFilterMode = GL_LINEAR_MIPMAP_LINEAR;
				else
					minFilterMode = GL_LINEAR_MIPMAP_LINEAR;
			} else { minFilterMode = (int32_t)filterMode; }

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int32_t)filterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int32_t)wrapMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int32_t)wrapMode);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, 
				GL_RGBA, GL_UNSIGNED_BYTE, (const void*) image.data);
			if(generateMipMaps) glGenerateMipmap(GL_TEXTURE_2D);

		}

		void set_texid(texslot_t id) const {
			glActiveTexture(GL_TEXTURE0 + id);
			glBindTexture(GL_TEXTURE_2D, m_GlId);
		}

		void bind() { glBindTexture(GL_TEXTURE_2D, m_GlId); }

		uint32_t get_renderer_id() const {
			return m_GlId;
		}

		/* ASSET CODE */

		using AssetParams = TextureAssetParams;

		static std::optional<Texture2D> construct_asset(const std::string& path, AssetParams params) {
			if(auto image = Image::open(path.c_str())) {
				Texture2D texture{*image, params.generateMipMaps, params.mipMapFilterMode, params.filterMode, params.wrapMode};
			}

			return std::nullopt;
		}

	private:
		uint32_t m_GlId;
	};
}
