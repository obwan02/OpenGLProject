#pragma once
#include <math.h>
#include "core.h"

namespace ogl {
	template<typename T>
	OGL_FORCE_INLINE T sqrt(T) { ogl::log::error("Type provided cannot be square rooted."); }

	template<>
	OGL_FORCE_INLINE float sqrt<float>(float value) {
		return ::sqrtf(value);
	}

	template<>
	OGL_FORCE_INLINE double sqrt<double>(double value) {
		return ::sqrt(value);
	}

	template<>
	OGL_FORCE_INLINE long double sqrt<long double>(long double value) {
		return ::sqrtl(value);
	}
}