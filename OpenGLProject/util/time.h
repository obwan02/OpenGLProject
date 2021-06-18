#pragma once
#include <oglpch.h>

namespace ogl {

	struct Time;
	struct Duration {
		Duration(float val) : value(val) { }
		operator float() { return value; }

		friend Time operator+(const Time& base, const Duration& d);
		friend Time operator-(const Time& base, const Duration& d);

		float value;
	};
	using DeltaTime = Duration;

	struct Time {
		inline static Time GetTime() { return Time((float)glfwGetTime()); }
		
		Time(float val) : value(val) { }
		operator float() { return value; }
		Duration operator-(const Time& other) { return Duration(value - other.value); }

		float value;
	};

	Time operator+(const Time& base, const Duration& d) {
		return Time(base.value + d.value);
	}

	Time operator-(const Time& base, const Duration& d) {
		return Time(base.value - d.value);
	}
}