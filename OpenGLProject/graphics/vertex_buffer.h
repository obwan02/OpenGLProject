#pragma once
#include "buffer.h"
#include "gl_data_types.h"

namespace ogl {

	struct VertexBufferFormatItem {
		ShaderDataType type; // it's value will also determine it's component count
		uint32_t size;
		bool normalised;

		size_t offset; // filled out by VertexBufferFormat
	};

	struct VertexBufferFormat {
		std::vector<VertexBufferFormatItem> items;

		VertexBufferFormat(std::initializer_list<VertexBufferFormatItem> il) : items(il) { 
			size_t offset = 0;
			for (auto& i : items) {
				i.offset = offset;
				offset += i.size;
				m_Stride += i.size;
			}

			auto data = items[1];
		}

		uint32_t Stride() const {
			return m_Stride;
		}

	private:
		uint32_t m_Stride;
	};

	class VertexBuffer : public Buffer<VertexBufferType::ArrayBuffer> {
	public:
		void SetFormat(const VertexBufferFormat& format) { m_Format = format;  }
		const VertexBufferFormat& GetFormat() const { return m_Format; };
	private:
		VertexBufferFormat m_Format;
	};
}