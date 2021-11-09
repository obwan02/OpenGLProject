#pragma once

#include <glad/glad.h>

#include "core.h"
#include "util/memview.h"

namespace ogl {

	// TODO: fill out
	enum class BufferType : GLenum {
		VertexBuffer = GL_ARRAY_BUFFER,
		IndexBuffer = GL_ELEMENT_ARRAY_BUFFER
	};

	// Rare = Modified once, not used much by GPU
	// Static = Modified once, used a lot by GPU
	// Dynamic = Modified lots, used a lot by GPU
	// 
	// Draw = Modified by application, used by GPU
	// Read = Modified by GPU, used by application
	// NoTouchy = Modified by GPU, used by GPU
	//
	// Do not rely on these values being mapped to GL values
	// just use the provided BufferUsageTypeToGL func.
	enum class BufferUsage : GLenum {
		RareDraw = GL_STREAM_DRAW, 
		StaticDraw = GL_STATIC_DRAW,
		DynamicDraw = GL_DYNAMIC_DRAW,

		RareRead = GL_STREAM_READ,
		StaticRead = GL_STREAM_READ,
		DynamicRead= GL_DYNAMIC_READ,

		RareNoTouchy = GL_STREAM_COPY,
		StaticNoTouchy = GL_STATIC_COPY,
		DynamicNoTouchy = GL_DYNAMIC_COPY,
	};

	enum class BufferMapHint : GLenum {
		ReadOnly = GL_READ_ONLY,
		WriteOnly = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE
	};

	constexpr GLenum BufferTypeToGL(BufferType bt) { return (GLenum) bt; }
	constexpr GLenum BufferUsageToGL(BufferUsage bu) { return (GLenum) bu; }
	constexpr GLenum BufferMapHintToGL(BufferMapHint bmh) { return (GLenum) bmh; }

	template<BufferType BT>
	class Buffer {
	public:
		Buffer(size_t byte_size, BufferUsage bu);
		Buffer(void* data, size_t byte_size, BufferUsage bu);
		Buffer(const Buffer<BT>&) = delete;
		Buffer(Buffer<BT>&& other);
		virtual ~Buffer();

		void bind();
		void unbind();
		void send_data(void* data, size_t byte_size, size_t byte_offset);
		void orphan();
		
		template<typename T> MemView<T> map_buffer(BufferMapHint hint);
		void unmap_buffer();

		// returns the size of the buffer in bytes
		size_t size() const { return m_Size; }

	private:
		uint32_t m_GlId;
		const BufferUsage m_Usage;
		size_t m_Size;
	};

	template<BufferType BT>
	Buffer<BT>::Buffer(size_t size, BufferUsage bu) : m_Usage(bu), m_Size(size) {
		glGenBuffers(1, &m_GlId);
		bind();
		glBufferData(BufferTypeToGL(BT), size, nullptr, BufferUsageToGL(bu));
	}

	template<BufferType BT>
	Buffer<BT>::Buffer(void* data, size_t byte_size, BufferUsage bu) : m_Usage(bu), m_Size(byte_size) {
		glGenBuffers(1, &m_GlId);
		bind();
		glBufferData(BufferTypeToGL(BT), m_Size, data, BufferUsageToGL(bu));
	}

	template<BufferType BT>
	Buffer<BT>::Buffer(Buffer<BT>&& other) : m_GlId(other.m_GlId) {
		other.m_GlId = 0;
	}
	
	template<BufferType BT>
	Buffer<BT>::~Buffer() {
		glDeleteBuffers(1, &m_GlId);
	}

	template<BufferType BT>
	void Buffer<BT>::bind() { glBindBuffer(BufferTypeToGL(BT), m_GlId); }

	template<BufferType BT>
	void Buffer<BT>::unbind() { glBindBuffer(BufferTypeToGL(BT), 0); }

	// send_data will send the specified data to the buffer in the GPU. The offset specified
	// is the offset in bytes into the buffer.
	template<BufferType BT>
	void Buffer<BT>::send_data(void* data, size_t byte_size, size_t byte_offset) {
#if OGL_DEBUG
		if(m_Usage != BufferUsage::DynamicDraw) { OGL_DEBUG_WARN("send_data is not recommended for usages other than DynamicDraw."); }
#endif
		OGL_DEBUG_ASSERT(byte_offset + byte_size <= m_Size);

		bind();
		glBufferSubData(BufferTypeToGL(BT), byte_offset, byte_size, data); 
		unbind();
	}

	template<BufferType BT>
	void Buffer<BT>::orphan() {
		// TODO: Some usage checks should probably be done here
		bind();
		glBufferData(BufferTypeToGL(BT), m_Size, nullptr, BufferUsageToGL(m_Usage));
		unbind();
	}

	// TODO: maybe use blobify for mapping data? If not at least magic-get?

	template<BufferType BT>
	template<typename T>
	MemView<T> Buffer<BT>::map_buffer(BufferMapHint hint) {
		bind();
		void* ptr = glMapBuffer(BufferTypeToGL(BT), BufferMapHintToGL(hint));
		unbind();

		OGL_DEBUG_ASSERT(m_Size % sizeof(T) == 0);
		OGL_DEBUG_ASSERT((size_t)ptr % alignof(T) == 0);

		size_t count = m_Size / sizeof(T);
		T* tptr = ::new (ptr) T[m_Size / sizeof(T)];
		return MemView<T>(tptr, count);
	}

	template<BufferType BT>
	void Buffer<BT>::unmap_buffer() {
		bind();
		glUnmapBuffer(BufferTypeToGL(BT));
		unbind();
	}

	class VertexBuffer : public Buffer<BufferType::VertexBuffer> {
	public:
		VertexBuffer(size_t size, BufferUsage bu) : Buffer(size, bu) {}
		VertexBuffer(void* data, size_t byte_size, BufferUsage bu) : Buffer(data, byte_size, bu) {}
	};

	class IndexBuffer : public Buffer<BufferType::IndexBuffer> { 
	private:
		using super = Buffer<BufferType::IndexBuffer>;
	public:
		using index_type = uint32_t;
		IndexBuffer(size_t size, BufferUsage bu) : Buffer(size * sizeof(index_type), bu) {}
		IndexBuffer(index_type* data, size_t count, BufferUsage bu) : Buffer(data, count * sizeof(index_type), bu) {}
		
		MemView<index_type> map_indices(BufferMapHint hint) { return super::map_buffer<index_type>(hint); }
		void unmap_indices() { super::unmap_buffer(); }
		void send_indices(index_type* data, size_t count, size_t offset) {
			super::send_data(data, count * sizeof(index_type), offset * sizeof(index_type)); 
		}
	};
}
