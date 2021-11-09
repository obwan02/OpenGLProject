#pragma once

#include <variant>
#include <string>
#include <optional>
#include <cstdio>
#include <fstream>

#include "core.h"

namespace ogl {

	std::optional<std::string> ReadFile(FILE* file);
	std::optional<std::string> ReadFile(const char* filepath);

}
