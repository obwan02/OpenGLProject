#pragma once
#include "log.h"

#define EXPAND_M(m) m

#ifdef OGL_DEBUG
	#ifdef _WIN64
		#define OGL_DEBUG_BREAK() __debugbreak()
	#elif defined(__linux__)
		#include <signal.h>
		#define OGL_DEBUG_BREAK() raise(SIGTRAP)
	#else
		#define OGL_DEBUG_BREAK()
	#endif

	#define OGL_DEBUG_INFO(...) log::InfoFrom("DEBUG", __VA_ARGS__)
	#define OGL_DEBUG_WARN(...) log::WarnFrom("DEBUG", __VA_ARGS__)
#else
	#define OGL_DEBUG_BREAK() exit(-1)
	#define OGL_DEBUG_INFO(...)
	#define OGL_DEBUG_WARN(...)
#endif
