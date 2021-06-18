#pragma once
#include <oglpch.h>
#include "vertex_buffer.h"

namespace ogl {

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void Bind();
		void Unbind();

		void AddVertexBuffer(const VertexBuffer& vb);

	private:
		GLuint m_GlId;
		uint32_t m_AttributeIndex = 0;
	};
};