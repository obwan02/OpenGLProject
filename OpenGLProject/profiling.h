#pragma once
#include <string_view>
#include <chrono>
#include <atomic>

#include "core.h"

namespace ogl {

	class Profiler {
		static void init() {

		}
		static void destroy() {

		}

		static void start() {
			m_ShouldProfile.store(true, std::memory_order_release);
		}

		static void stop() {

		}

		static void dump() {

		}

		static void submit() {
			if (m_ShouldProfile.load(std::memory_order_aquire)) {

			}
		}


	private:
		static std::atomic<bool> m_ShouldProfile;
	};

	struct ProfileScope {
		using duration_t = std::chrono::duration<float, std::micro>;

		ProfileScope(const char* scopeName) : name(scopeName) {
			using namespace std::chrono;
			msStart = duration_cast<duration_t>(high_resolution_clock::now());
		}

		~ProfileScope() {
			using namespace std::chrono;
			msEnd = duration_cast<duration_t>(high_resolution_clock::now());

			Profiler::submit(name, msStart, msEnd);
		}

		const char* name;
		float msStart;
		float msEnd;
	};

	
}