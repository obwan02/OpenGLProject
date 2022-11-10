#pragma once
#include <chrono>
#include <GLFW/glfw3.h>

#include "core.h"

namespace ogl {

	struct Time;
	struct Duration {
		Duration(float val) : value(val) { }
		
		operator float() { return value; }

		friend Time operator+(const Time& base, const Duration& d);
		friend Time operator-(const Time& base, const Duration& d);
		
		const float value;
	};
	using DeltaTime = Duration;

	struct Time {
		Time(float val) : value(val) { }
		operator float() { return value; }
		Duration operator-(const Time& other) { return Duration(value - other.value); }

		const float value;
	};

	inline Time operator+(const Time& base, const Duration& d) {
		return Time(base.value + d.value);
	}

	inline Time operator-(const Time& base, const Duration& d) {
		return Time(base.value - d.value);
	}

	inline Time get_time() { return Time((float)glfwGetTime()); }
}
