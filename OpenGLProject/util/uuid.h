#pragma once
#include "errors.h"

#define GEN_BAD_UUID 

namespace ogl {
	using UUID_t = unsigned char[16];
	namespace uuid {
		void Generate(UUID_t u);
	}
}
