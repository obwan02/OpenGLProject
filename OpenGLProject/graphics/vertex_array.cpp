#include "vertex_array.h"

namespace ogl {
	
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_GlId);
	}
	
	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_GlId);
	}

	void VertexArray::set_index_buffer(IndexBuffer& buffer) {
			bind();
			buffer.bind();
			unbind();
			buffer.unbind();
	}

	void VertexArray::draw_indices(uint32_t numIndices, uint32_t offset) {
		bind();
		glDrawElements(GL_TRIANGLES, numIndices, TypeToGLAttributeData<IndexBuffer::index_type>().glEnumType, (const void*)offset);
		unbind();
	}
}
