#pragma once

#include <glad/glad.h>

#include "core.h"
#include "buffer.h"
#include "math/vector.h"

namespace ogl {

	namespace intern {
		struct GLAttributeData {
			int32_t numComponents;
			GLenum glEnumType;
		};
	}

	template<typename T> constexpr intern::GLAttributeData TypeToGLAttributeData();

	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<int8_t>() { return {1, GL_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<uint8_t>() { return {1, GL_UNSIGNED_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<int8_t>>() { return {2, GL_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<uint8_t>>() { return {2, GL_UNSIGNED_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<int8_t>>() { return {3, GL_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<uint8_t>>() { return {3, GL_UNSIGNED_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<int8_t>>() { return {4, GL_BYTE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<uint8_t>>() { return {4, GL_UNSIGNED_BYTE}; }

	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<int16_t>() { return {1, GL_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<uint16_t>() { return {1, GL_UNSIGNED_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<int16_t>>() { return {2, GL_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<uint16_t>>() { return {2, GL_UNSIGNED_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<int16_t>>() { return {3, GL_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<uint16_t>>() { return {3, GL_UNSIGNED_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<int16_t>>() { return {4, GL_SHORT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<uint16_t>>() { return {4, GL_UNSIGNED_SHORT}; }

	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<int32_t>() { return {1, GL_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<uint32_t>() { return {1, GL_UNSIGNED_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<int32_t>>() { return {2, GL_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<uint32_t>>() { return {2, GL_UNSIGNED_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<int32_t>>() { return {3, GL_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<uint32_t>>() { return {3, GL_UNSIGNED_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<int32_t>>() { return {4, GL_INT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<uint32_t>>() { return {4, GL_UNSIGNED_INT}; }

	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<float>() { return {1, GL_FLOAT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<double>() { return {1, GL_DOUBLE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<float>>() { return {2, GL_FLOAT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector2<double>>() { return {2, GL_DOUBLE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<float>>() { return {3, GL_FLOAT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector3<double>>() { return {3, GL_DOUBLE}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<float>>() { return {4, GL_FLOAT}; }
	template<> constexpr intern::GLAttributeData TypeToGLAttributeData<Vector4<double>>() { return {4, GL_DOUBLE}; }

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		inline void bind() { glBindVertexArray(m_GlId); }
		inline void unbind() {
			glBindVertexArray(0); 
		}

		template<typename T>
		void set_attrib(uint32_t id, 
						VertexBuffer& buffer,	
						size_t byte_stride, 
						size_t byte_offset,
						bool normalised = false); 

		void set_index_buffer(IndexBuffer& buffer);
		void draw_indices(uint32_t numIndices, uint32_t offset);

	private:
		uint32_t m_GlId;
	};

	template<typename T>
	void VertexArray::set_attrib(uint32_t id, VertexBuffer& buffer, size_t byte_stride, size_t byte_offset, bool normalised) {
		buffer.bind();
		this->bind();

		constexpr auto data = TypeToGLAttributeData<T>();		
		GLboolean normal = normalised ? GL_TRUE : GL_FALSE;
		glEnableVertexAttribArray(id);
		
		if constexpr (std::numeric_limits<T>::is_integer) {
			glVertexAttribIPointer(id, data.numComponents, data.glEnumType, byte_stride, (const void*) byte_offset);
		}
		else {
			glVertexAttribPointer(id, data.numComponents, data.glEnumType, normal, byte_stride, (const void*) byte_offset);
		}

		this->unbind();
		buffer.unbind();
	}
}
