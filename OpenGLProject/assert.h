#pragma once
#include <string>
#include <string_view>

#include "log.h"
#include "debug.h"

#define EXPAND_M(m) m

#ifdef OGL_ENABLE_ASSERTS
#define OGL_ASSERT_IMPL_NO_MSG(x) if(!(x)) {  ogl::log::Error("Assert failed: '" #x "' in ", ogl::assert_internal::filename(__FILE__), ", Line ", __LINE__); OGL_DEBUG_BREAK(); }
#define OGL_ASSERT_IMPL_MSG(x, msg) if(!(x)) { ogl::log::Error("Assert failed: '", msg, "' in ", ogl::assert_internal::filename(__FILE__), " Line ",  __LINE__); OGL_DEBUG_BREAK(); }
#define OGL_ASSERT_GET_IMPL(a, b, macro, ...) macro
#define OGL_ASSERT(...) EXPAND_M(OGL_ASSERT_GET_IMPL(__VA_ARGS__, OGL_ASSERT_IMPL_MSG, OGL_ASSERT_IMPL_NO_MSG)(__VA_ARGS__))
#if OGL_DEBUG
#define OGL_DEBUG_ASSERT(...) OGL_ASSERT(__VA_ARGS__);
#else
#define OGL_DEBUG_ASSERT(...)
#endif
#else
#define OGL_ASSERT(...) 
#endif

namespace ogl {
	namespace assert_internal {
		inline std::string_view filename(std::string_view path) {
			size_t backSlashSearch = path.rfind('\\');
			size_t fowardSlashSearch = path.rfind('/');
			if (backSlashSearch != std::string::npos && fowardSlashSearch != std::string::npos) {
				return (backSlashSearch > fowardSlashSearch) ? path.substr(backSlashSearch + 1) : path.substr(fowardSlashSearch + 1);
			}
			else if (backSlashSearch != std::string::npos) {
				return path.substr(backSlashSearch + 1);
			}
			else if (fowardSlashSearch != std::string::npos) {
				return path.substr(fowardSlashSearch + 1);
			}

			return path;
		}
	}
}
