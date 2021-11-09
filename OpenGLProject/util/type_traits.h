#pragma once
#include <string_view>

#include "core.h"
#include "string_hash.h"

#ifdef _MSC_VER
	#define OGL_FUNC_SIG __FUNCSIG__
	#define FUNCSIG_TYPE_PREFIX "<"
	#define FUNCSIG_TYPE_SUFFIX ">"
	#define FUNCSIG_TYPE_OFFSET 79
#elif defined(__clang__) || defined(__GNUC__)
	#define OGL_FUNC_SIG __PRETTY_FUNCTION__
	#define FUNCSIG_TYPE_PREFIX "["
	#define FUNCSIG_TYPE_SUFFIX ";"
	#define FUNCSIG_TYPE_OFFSET 0
#endif

namespace ogl {

	namespace intern {

		inline OGL_NO_DISCARD id_t type_seq_next() {
			static id_t value{};
			return value++;
		}

		template<typename T>
		inline constexpr std::string_view type_name() {
			constexpr std::string_view sv{OGL_FUNC_SIG};
			constexpr size_t prefixIndex = sv.find_first_of(FUNCSIG_TYPE_PREFIX, FUNCSIG_TYPE_OFFSET);
			constexpr size_t suffixIndex = sv.find_last_of(FUNCSIG_TYPE_SUFFIX);
			return sv.substr(prefixIndex + 1, suffixIndex - prefixIndex - 1);
		}

	}

	template<typename T>
	struct type_traits {
		static constexpr std::string_view name() {
			return intern::type_name<T>();
		}

		// Returns the sequential identifier of a given type.
		static id_t seq() {
			static const id_t value = intern::type_seq_next();
			return value;
		}

		// Returns a unique hash of a given type
		static constexpr id_t hash() {
			return StringHash(name());
		}
	};

}
