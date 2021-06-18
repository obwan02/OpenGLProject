//Credit To "The Cherno" for the AddVertexBuffer function. 
//This function is essentially a lightly modified version of the function from github.com/TheCherno/Hazel
#include "vertex_array.h"

namespace ogl {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_GlId);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_GlId);
	}

	void VertexArray::Bind() {
		glBindVertexArray(m_GlId);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const VertexBuffer& vb) {
		Bind();
		vb.Bind();
		const VertexBufferFormat& format = vb.GetFormat();

		for (auto item : format.items) {
			switch (item.type) {
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_AttributeIndex);
				glVertexAttribPointer(m_AttributeIndex,
					ShaderDataTypeToComponentCount(item.type),
					ShaderDataTypeToGLType(item.type),
					item.normalised ? GL_TRUE : GL_FALSE,
					format.Stride(),
					(const void*)item.offset);
				m_AttributeIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_AttributeIndex);
				glVertexAttribIPointer(m_AttributeIndex,
					ShaderDataTypeToComponentCount(item.type),
					ShaderDataTypeToGLType(item.type),
					format.Stride(),
					(const void*)item.offset);
				m_AttributeIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = ShaderDataTypeToComponentCount(item.type);
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_AttributeIndex);
					glVertexAttribPointer(m_AttributeIndex,
						count,
						ShaderDataTypeToGLType(item.type),
						item.normalised ? GL_TRUE : GL_FALSE,
						format.Stride(),
						(const void*)(item.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_AttributeIndex, 1);
					m_AttributeIndex++;
				}
				break;
			}
			default:
				OGL_ASSERT(false, "Unknown shader data type");
			}
		}

		Unbind();
		vb.Unbind();
	}
}