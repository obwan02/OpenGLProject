#pragma once
#include <utility>
#include <vector>

#include "core.h"

namespace ogl {

	template<typename T>
	class Asset {

		virtual std::vector<char> serialize() = 0;
		static T deserialize() { return T::deserialize(); }
	};
}