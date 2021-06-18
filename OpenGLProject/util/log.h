#pragma once
#include <oglpch.h>

#ifdef OGL_ENABLE_WARNINGS 
	#define OGL_WARN(...) ::ogl::LogImpl::PrintWarningLn(__VA_ARGS__)
#else
	#define OGL_WARN(...)
#endif

#ifdef OGL_GL_VERBOSE
	#define OGL_GL_VERBOSE_LOG(...) ::ogl::LogImpl::PrintGLVerboseLn(__VA_ARGS__)
#else
	#define OGL_GL_VERBOSE_LOG(...)
#endif

#ifdef OGL_SHOW_INFO
	#define OGL_INFO(...) ::ogl::LogImpl::PrintInfoLn(__VA_ARGS__)
#else
	#define OGL_INFO(...)
#endif

#define OGL_STATUS(...) ::ogl::LogImpl::PrintStatusLn(__VA_ARGS__)
#define OGL_PRINT(...) ::ogl::LogImpl::Print(__VA_ARGS__)
#define OGL_PRINT_LN(...) ::ogl::LogImpl::PrintLn(__VA_ARGS__)

namespace ogl {
	namespace LogImpl {

		template<typename T>
		inline void Print(T&& first) {
			std::cout << std::forward<T>(first);
		}

		template<typename T, typename ...Args>
		inline void Print(T&& first, Args&&... rest) {
			std::cout << std::forward<T>(first);
			Print<Args...>(std::forward<Args>(rest)...);
		}

		template<typename ...Args>
		inline void PrintLn(Args&&... args) {
			Print<Args...>(std::foward<Args>(args)...);
			std::cout << '\n';
		}

		template<typename ...Args>
		inline void PrintStatusLn(Args&&... args) {
			PrintLn(" -- ", std::foward<Args>(args)...);
		}

		//OGL_SHOW_INFO must be defined
		template<typename ...Args>
		inline void PrintInfoLn(Args&&... args) {
	#ifdef OGL_SHOW_INFO
			PrintLn("[INFO] ", std::foward<Args>(args)...);
	#endif
		}

		//OGL_GL_VERBOSE must be defined
		template<typename ...Args>
		inline void PrintGLVerboseLn(Args&&... args) {
	#ifdef OGL_GL_VERBOSE
			PrintLn("[GL VERBOSE] ", std::foward<Args>(args)...);
	#endif
		}
		template<typename ...Args>
		inline void PrintWarningLn(Args&&... args) {
	#ifdef OGL_ENABLE_WARNINGS
			PrintLn("[WARNING] ", std::foward<Args>(args)...);
	#endif
		}
	}
}