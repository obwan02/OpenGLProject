#pragma once

#include <optional>
#include <cstdio>

#include "core.h"

namespace ogl {

	std::optional<std::string> ReadFile(FILE* file);
	std::optional<std::string> ReadFile(const char* filepath);

}
