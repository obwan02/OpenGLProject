#pragma once

#include "assert.h"
#include <algorithm>
#define STBI_FREE(x) free(x)
#define STBI_MALLOC(x) malloc(x)
#define STBI_REALLOC(ptr, size) realloc(ptr, size)
#define STBIW_FREE(x) free(x)
#define STBIW_MALLOC(x) malloc(x)
#define STBIW_REALLOC(ptr, size) realloc(ptr, size)

#include <optional>
#include <cstring>
#include <execution>

#include "core.h"
#include "math/vector.h"
#include "util/stb_image.h"
#include "util/stb_image_write.h"

namespace ogl {

	// TODO: Write move assignment operator

	struct Image {

		using pixel_t = Vector4<uint8_t>;
		using iterator = pixel_t*;
		using const_iterator = pixel_t*;
	private:
		Image(pixel_t* data, int width, int height) : data(data), width(width), height(height) {}
	public:
	
		Image(const Image& other) : width(other.width), height(other.height) {
			size_t size = (size_t)width * (size_t)height;
			data = new(STBI_MALLOC(size * sizeof(pixel_t))) pixel_t[size]; 
			std::copy_n(std::execution::par_unseq, other.data, width * height, data);
		}

		Image(Image&& other) : data(other.data), width(other.width), height(other.height) { other.data = nullptr; }
		// This constructor will copy the data pointed to by the data ptr. Memory will be allocated
		Image(int width, int height, const pixel_t* data = nullptr) : width(width), height(height) {
			size_t size = (size_t)width * (size_t)height;
			this->data = new(STBI_MALLOC(size * sizeof(pixel_t))) pixel_t[size]; 
			if (data != nullptr)
				std::memcpy(this->data, data, size * sizeof(pixel_t));
		}

		~Image() {
			if(data) stbi_image_free(data); 
		}

		bool write(const char* path) {
			stbi_flip_vertically_on_write(1);
			return stbi_write_png(path, width, height, 4, data, 0);
		}

		static std::optional<Image> Open(FILE* file) {
			stbi_set_flip_vertically_on_load(1);

			int channels, width, height;
			unsigned char* pre_data = stbi_load_from_file(file, &width, &height, &channels, 4);
			if (pre_data == nullptr) {
				log::Error("STBI Failed to read image file");
				return std::nullopt;
			}

			size_t size = (size_t)width * (size_t)height;
			pixel_t* data = new(STBI_MALLOC(size * sizeof(pixel_t))) pixel_t[size];
			std::memcpy(data, pre_data, size * sizeof(pixel_t));
			stbi_image_free(pre_data);

			return Image(data, width, height);
		}

		static std::optional<Image> Open(const char* file) {
			FILE* fp = fopen(file, "rb");
			if(!fp) { 
				log::Error("Failed to open image '", file, "' for reading");
				return std::nullopt;
			}
			SCOPE_DEFER([fp] { fclose(fp);  });

			return Image::Open(fp);
		}

		int width, height;
		pixel_t* data;
	};

}
