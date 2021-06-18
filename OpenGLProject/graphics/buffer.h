#pragma once
#include <oglpch.h>
#include "../util/debug.h"

namespace ogl {

	enum class VertexBufferType : GLenum {
		ArrayBuffer = GL_ARRAY_BUFFER,
		AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
		CopyReadBuffer = GL_COPY_READ_BUFFER,
		CopyWriteBuffer = GL_COPY_WRITE_BUFFER,
		ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
		TextureBuffer = GL_TEXTURE_BUFFER,
		UniformBuffer = GL_UNIFORM_BUFFER
	};

	enum class VertexBufferUsage : GLenum {
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StreamDraw = GL_STREAM_DRAW,

		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		StaticDraw = GL_STATIC_DRAW,

		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW
	};

	template<VertexBufferType VBT> // generic buffer type 
	class Buffer {
	protected:
		Buffer() {
			glGenBuffers(1, &m_GlId);
		}

		Buffer(GLsizeiptr size, VertexBufferUsage usage = VertexBufferUsage::DynamicDraw) 
			: m_Size(size), m_Usage(usage) {
			glGenBuffers(1, &m_GlId);
			BufferData(size, nullptr, usage);
		}

	public:
		void Bind() const {
			glBindBuffer((GLenum)VBT, m_GlId);
		}

		void Unbind() const {
			glBindBuffer((GLenum)VBT, 0);
		}

		// Will delete pre-existing data and assign new dat ato this buffer
		void BufferData(GLsizeiptr size, const void* data, VertexBufferUsage usage = VertexBufferUsage::DynamicDraw) {
			Bind();
			glBufferData((GLenum)VBT, size, data, usage);
			Unbind();
		}

		void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) {
			Bind();
			glBufferSubData((GLenum)VBT, offset, size, data);
			Unbind();
		}

		// Reallocate assigns a new buffer position
		// and gets rid of all data
		void Reallocate() {
			Bind();

			Unbind();
		}

	private:
		GLuint m_GlId;
		GLsizeiptr m_Size;
		VertexBufferUsage m_Usage;
	};
}