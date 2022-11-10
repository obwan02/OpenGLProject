#pragma once

#include "core.h"
#include "tex_coords.h"
#include "math/vector.h"
#include "graphics/texture.h"

namespace ogl {

	struct RendererSpriteData {

		RendererSpriteData(Vector3f* p, Vector2f* s, Vector4f* c, TexCoords* tc, std::shared_ptr<Texture2D>* ti, size_t count) 
			: pos(p), size(s), col(c), texCoords(tc), texture(ti), spriteCount(count) {}

		RendererSpriteData offset(size_t count) const {
			OGL_DEBUG_ASSERT(count <= spriteCount);
			return RendererSpriteData(pos + count, size + count, col + count, 
					texCoords + count, texture + count, spriteCount - count);
		}

		RendererSpriteData subset(size_t offset, size_t count) const {
			auto data = this->offset(offset);
			OGL_DEBUG_ASSERT(count <= data.spriteCount);
			data.spriteCount = count;
			return data;
		}

		RendererSpriteData operator++(int) {
			auto ret = *this;
			pos++; size++; col++; texCoords++; texture++;
			return ret;
		}

		Vector3f* pos;
		Vector2f* size;
		Vector4f* col;
		TexCoords* texCoords;
		std::shared_ptr<Texture2D>* texture;

		size_t spriteCount;
	};

}
