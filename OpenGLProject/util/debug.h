#pragma once
#include <oglpch.h>
#include "log.h"

#define EXPAND_M(m) m

#ifdef OGL_DEBUG
	#ifdef _WIN64
		#define OGL_DEBUGBREAK() __debugbreak()
	#elif defined(__linux__)
		#include <signal.h>
		#define HZ_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define OGL_DEBUGBREAK() exit(-1)
#endif

#ifdef OGL_ENABLE_ASSERTS
	#define OGL_ASSERT_IMPL_NO_MSG(x) ::ogl::DebugImpl::Assert(x, #x " At " + ::ogl::DebugImpl::Filename(__FILE__) + " Line " + std::to_string(__LINE__))
	#define OGL_ASSERT_IMPL_MSG(x, msg) ::ogl::DebugImpl::Assert(x, msg " At " + ::ogl::DebugImpl::Filename(__FILE__) + " Line " + std::to_string(__LINE__))
	#define OGL_ASSERT_GET_IMPL(a, b, macro, ...) macro
	#define OGL_ASSERT(...) EXPAND_M(OGL_ASSERT_GET_IMPL(__VA_ARGS__, OGL_ASSERT_IMPL_MSG, OGL_ASSERT_IMPL_NO_MSG)(__VA_ARGS__))
#else
	#define OGL_ASSERT(x) 
#endif

#ifdef OGL_ENABLE_ERRORS
	#define OGL_ERROR(x) ::ogl::DebugImpl::Error(x)
#else 
	#define OGL_ERROR(x)
#endif

#ifdef OGL_DEBUG
	#define OGL_DEBUG_INFO(...) OGL_PRINT_LN("[DEBUG] ", __VA_ARGS__)
#else
	#define OGL_DEBUG_INFO
#endif


namespace ogl {
	namespace DebugImpl {

		inline std::string Filename(const std::string& path) {
			size_t backSlashSearch = path.rfind('\\');
			size_t fowardSlashSearch = path.rfind('/');
			if (backSlashSearch != std::string::npos && fowardSlashSearch != std::string::npos) {
				return (backSlashSearch > fowardSlashSearch) ? path.substr(backSlashSearch) : path.substr(fowardSlashSearch);
			}
			else if (backSlashSearch != std::string::npos) {
				return path.substr(backSlashSearch);
			}
			else if (fowardSlashSearch != std::string::npos) {
				return path.substr(fowardSlashSearch);
			}

			return path;
		}

		template<typename ...Args>
		inline void Assert(bool condition, Args&&... args) {
			if (!condition) {
				OGL_PRINT("[ASSERT FAILED] ", std::foward<Args>(args)...);
				OGL_DEBUGBREAK();
			}
		}

		template<typename ...Args>
		inline void Error(Args&&... args) {
			OGL_PRINT("[ERROR] ", std::forward<Args>(args)...);
			OGL_DEBUGBREAK();
		}
	}
}