#pragma once
#include <string.h>
#include "core.h"

namespace ogl {

	inline constexpr size_t StringLength(const char* str) {
		size_t result = 0;
		for(; str[result] != '\0'; result++) {}
		return result;
	}

	inline constexpr id_t StringHash(const char* str, size_t len) {
		unsigned long myHash = 5381;
		while(len--) myHash = ((myHash << 5) + myHash) + *str++; /* hash * 33 + c */
		return myHash;
	}
		
	inline constexpr id_t StringHash(const char* str) {
		return StringHash(str, StringLength(str));
	}

	inline constexpr id_t StringHash(std::string_view str) {
		return StringHash(str.data(), str.size());
	}
}
