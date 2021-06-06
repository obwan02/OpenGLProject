#pragma once
#include <stdio.h>
#include <string.h>

#define OGL_DEBUG
#define OGL_SHOW_ERRORS

namespace ogl {
	struct Debug {
		Debug() = delete;

		static inline void Print(const std::string& str) {
			fputs(str.c_str(), stdout);
		}

		static inline void PrintLn(const std::string& str) {
			puts(str.c_str());
		}

		static inline void PrintDebug(const std::string& str) {
#ifdef OGL_DEBUG
			Print("[DEBUG]" + str);
#endif
		}

		static inline void PrintDebugLn(const std::string& str) {
#ifdef OGL_DEBUG
			PrintLn("[DEBUG]" + str);
#endif
		}

		static inline void PrintInfo(const std::string& str) {
#ifdef OGL_INFO
			Print("[INFO]" + str);
#endif
		}

		static inline void PrintInfoLn(const std::string& str) {
#ifdef OGL_INFO
			PrintLn("[INFO]" + str);
#endif
		}

		static inline void PrintGLVerbose(const std::string& str) {
#ifdef OGL_INFO
			Print("[GL VERBOSE]" + str);
#endif
		}

		static inline void PrintGLVerboseLn(const std::string& str) {
#ifdef OGL_INFO
			PrintLn("[GL VERBOSE]" + str);
#endif
		}

		static inline void PrintError(const std::string& str) {
#ifdef OGL_SHOW_ERRORS
			Print("[ERROR]" + str);
#endif
		}

		static inline void PrintErrorLn(const std::string& str) {
#ifdef OGL_SHOW_ERRORS
			PrintLn("[ERROR]" + str);
#endif
		}



	};
}