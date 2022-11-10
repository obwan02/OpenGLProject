#pragma once
#include <stdint.h>
#include <stdlib.h>


#if _MSC_VER
	#define OGL_PLATFORM_WINDOWS
	#if _WIN64
		#define OGL_64
	#elif _WIN32
		#define OGL_32
#endif

#elif defined(__clang__) || defined(__GNUC__)
	#ifndef __linux__
	#else
		#define OGL_PLATFORM_LINUX
	#endif 

	#if __x86_64__ || __ppc64__
		#define OGL_64
	#elif defined(__i386__)
		#define OGL_32
	#endif
#else
	#error "Compiler not supported!"
#endif

namespace ogl {
#ifdef OGL_64
	using id_t = uint64_t;
#else
	using id_t = uint32_t;
#endif
}

#if __cplusplus >= 201710L //C++17
	#define OGL_NO_DISCARD [[nodiscard]]
#else 
	#define OGL_NO_DISCARD
#endif

#ifndef CACHE_LINE_SIZE // Should be set in compiler defs
#define CACHE_LINE_SIZE 128
#endif

#if defined(__clang__) || defined(__GNUC__)
	#define OGL_FORCE_INLINE __attribute__((always_inline))
	#define OGL_RESTRICT __restrict
#elif _MSC_VER
	#define OGL_FORCE_INLINE __forceinline
	#define OGL_RESTRICT __restrict
#else
	#error "Compiler not supported!"
#endif

#define OGL_TARGET_GL_MAJOR 4
#define OGL_TARGET_GL_MINOR 3

#include "assert.h"
#include "log.h"
#include "debug.h"
#include "defer.h"
