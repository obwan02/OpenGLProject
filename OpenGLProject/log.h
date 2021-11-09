#pragma once
#include <iostream>
#include <tuple>
#include <utility>
#include <string_view>
#include "util/text_colours.h"

#ifndef DEBUG_COLOUR
#define DEBUG_COLOUR TEXT_WHITE
#endif

#ifndef INFO_COLOUR
#define INFO_COLOUR TEXT_GREEN
#endif

#ifndef WARN_COLOUR
#define WARN_COLOUR TEXT_YELLOW
#endif

#ifndef ERROR_COLOUR
#define ERROR_COLOUR TEXT_RED
#endif

#ifndef CRITICAL_COLOUR
#define CRITICAL_COLOUR TEXT_RED
#endif

namespace ogl {
	namespace log {

		namespace intern {
			template<typename TupleT, size_t ...I>
			void PrintTuple(const TupleT& tup, std::index_sequence<I...>) {
				((std::cout << std::get<I>(tup)),...);
			}

			template<typename ...Args>
			void Print(Args&& ...args) {
				PrintTuple(std::make_tuple(std::forward<Args>(args)...), std::make_index_sequence<sizeof...(Args)>());
			}
		}

		template<typename ...Args>
		void Info(Args&& ...args) {
			std::cout << "[info] " INFO_COLOUR; 
			intern::Print(std::forward<Args>(args)...);
            std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void Warn(Args&& ...args) {
			std::cout << "[warn] " WARN_COLOUR;
			intern::Print(std::forward<Args>(args)...);
            std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void Error(Args&& ...args) {
			std::cout << "[warn] " ERROR_COLOUR;
			intern::Print(std::forward<Args>(args)...);
            std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void Critical(Args&& ...args) {
			std::cout << "[critical] " CRITICAL_COLOUR;
			intern::Print(std::forward<Args>(args)...);
            std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void InfoFrom(std::string_view place, Args&& ...args) {
			std::cout << "(" << place << ")[info] " INFO_COLOUR;
			intern::Print(std::forward<Args>(args)...);
			std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void WarnFrom(std::string_view place, Args&& ...args) {
			std::cout << "(" << place << ")[warn] " WARN_COLOUR;
			intern::Print(std::forward<Args>(args)...);
			std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void ErrorFrom(std::string_view place, Args&& ...args) {
			std::cout << "(" << place << ")[error] " ERROR_COLOUR;
			intern::Print(std::forward<Args>(args)...);
			std::cout << TEXT_RESET "\n";
		}

		template<typename ...Args>
		void CriticalFrom(std::string_view place, Args&& ...args) {
			std::cout << "(" << place << ")[critical] " CRITICAL_COLOUR;
			intern::Print(std::forward<Args>(args)...);
			std::cout << TEXT_RESET "\n";
		}

	}
}
