#include "renderer2d.h"
#include "graphics/context.h"
#include "math/matrix.h"
#include <algorithm>
#include <sstream>

// Vertices should be specified in a clockwise manner starting 
// at the bottom left corner:
//
//     1 *--------------* 2
//       |              |
//       |              |
//       |              |
//       |              |
//     0 *--------------* 3
//
// This means that the position for a sprite is the bottom left
// corner.

namespace ogl {
	
	static void CreateIndexBuffer(IndexBuffer& buffer) {	
	
		MemView<IndexBuffer::index_type> indices = buffer.map_indices(BufferMapHint::WriteOnly);

		IndexBuffer::index_type index = 0;
		for(size_t i = 0; i < OGL_2D_BATCH_MAX_INDICES; i += 6) {
			indices[i + 0] = index + 0;			
			indices[i + 1] = index + 1;			
			indices[i + 2] = index + 2;			
			indices[i + 3] = index + 2;			
			indices[i + 4] = index + 3;			
			indices[i + 5] = index + 0;	
			index += 4;
		}

		buffer.unmap_indices();
	}

	Renderer2D::Renderer2D(const GraphicsContext& context) : m_BatchVBO(OGL_2D_BATCH_MAX_VERTS * sizeof(SpriteVertex), BufferUsage::DynamicDraw),
		m_BatchIBO(OGL_2D_BATCH_MAX_INDICES, BufferUsage::StaticDraw), m_MaxTextureSlots(context.maxFragmentTextureSlots)
	{
		// Setup index
		CreateIndexBuffer(m_BatchIBO);

		ShaderBuilder builder;
		builder.add_vertex_shader(
			"#version 330 core\n"
			"in vec3 vert_pos;\n"
			"in vec4 vert_colour;\n"
			"in vec2 vert_texCoord;\n"
			"in uint vert_texId;\n"

			"out vec4 colour;\n"
			"out vec2 texCoord;\n"
			"flat out uint texId;\n"

			"uniform mat4 u_projection;"
			"\n"
			"void main() {\n"
			"	gl_Position = u_projection * vec4(vert_pos, 1.0);\n"
			"	colour = vert_colour;\n"
			"   texCoord = vert_texCoord;\n"
			"   texId = vert_texId;\n"
			"}\n"
		);

		std::stringstream fragShader;
		fragShader << "#version 330 core\n"
			"out vec4 frag_Colour;\n"
			"\n"
			"in vec4 colour;\n"
			"in vec2 texCoord;"
			"flat in uint texId;\n"
			"\n"
			"uniform sampler2D u_samplers[";
		fragShader << m_MaxTextureSlots/2;
		fragShader << "];\n"
			"\n"
			"void main() {\n"
			"   vec4 textureColour = texture(u_samplers[texId], texCoord);\n"
			"	if(textureColour.w > 0) {\n"
			"        frag_Colour = colour * textureColour;\n"
			"    } else {\n"
			"        frag_Colour = colour;"
			"    }\n"
			"}\n";

		builder.add_fragment_shader(fragShader.str());

		m_VAO.set_index_buffer(m_BatchIBO);
		constexpr uint32_t vertPosIndex      = 0;
		constexpr uint32_t vertColIndex      = 1;
		constexpr uint32_t vertTexCoordIndex = 2;
		constexpr uint32_t vertTexIdIndex    = 3;

		m_VAO.set_attrib<Vector3f>(vertPosIndex,      m_BatchVBO, sizeof(SpriteVertex), 0);
		m_VAO.set_attrib<Vector4f>(vertColIndex,      m_BatchVBO, sizeof(SpriteVertex), sizeof(Vector3f));
		m_VAO.set_attrib<Vector2f>(vertTexCoordIndex, m_BatchVBO, sizeof(SpriteVertex), sizeof(Vector3f) + sizeof(Vector4f));
		m_VAO.set_attrib<texslot_t>(vertTexIdIndex,     m_BatchVBO, sizeof(SpriteVertex), sizeof(Vector3f) + sizeof(Vector4f) + sizeof(Vector2f));
		builder.specify_attrib(vertPosIndex, "vert_pos");
		builder.specify_attrib(vertColIndex, "vert_colour");
		builder.specify_attrib(vertTexCoordIndex, "vert_texCoord");
		builder.specify_attrib(vertTexIdIndex, "vert_texId");

		auto shader = builder.generate();
		if(!shader) { OGL_ASSERT(false, "Failed to generate shader."); }
			m_Shader = std::make_unique<Shader>(std::move(*shader));;

		ogl::log::InfoFrom("Renderer2D", "Using ", m_MaxTextureSlots, " texture slots");

		// Setup samplers
		{
			std::vector<int32_t> slots;
			slots.reserve(m_MaxTextureSlots);
			for (int32_t i = 0; i < m_MaxTextureSlots; i++) {
				slots.push_back(i);
			}

			m_Shader->set_uniform_array("u_samplers", slots.data(), slots.size());
		}
		
		m_BatchMappedVBO = m_BatchVBO.map_buffer<SpriteVertex>(BufferMapHint::WriteOnly);
	}

	Renderer2D::~Renderer2D() {}


	Matrix4f Renderer2D::calc_ortho_mat(const GraphicsContext& context) {
		const auto w = context.frameBufferWidth / 2.0f;
		const auto h = context.frameBufferHeight / 2.0f;
		return Matrix4f::Ortho(-w, w, -h, h, m_Near, m_Far);
	}

	void Renderer2D::flush(const GraphicsContext& context) {
		m_BatchVBO.unmap_buffer();

		const auto ortho = calc_ortho_mat(context);
		m_Shader->set_uniform("u_projection", ortho);
		m_Shader->bind();

		m_VAO.draw_indices(m_BatchSpriteCount * 6, 0);

		// Resetting batch counters
		m_BatchSpriteCount = 0;
		m_BatchTexIds.clear();
		m_BatchVBO.orphan();
		m_BatchMappedVBO = m_BatchVBO.map_buffer<SpriteVertex>(BufferMapHint::WriteOnly);
	}
}
