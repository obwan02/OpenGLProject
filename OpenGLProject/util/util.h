#pragma once
#include <oglpch.h>

#define OGL_BIND_CLASS_FUNCTION(x) ogl::PartialBindFunction(&x, this)

#include "errors.h"
#include "linked_list.h"
#include "debug.h"
#include "log.h"
#include "time.h"

namespace ogl {

	template<bool val> struct __opposite {};
	template<> struct __opposite<true> : std::false_type {};
	template<> struct __opposite<false> : std::true_type {};


	template<typename F, typename ...Args>
	struct __binder {

		__binder(F&& func, Args&&... args) : _internal(std::forward<F>(func)), bindArgs(std::forward<Args>(args)...) { }

		template<typename ...Args2>
		decltype(auto) operator()(Args2&&... args) const {
			constexpr auto seq = std::index_sequence_for<Args...>{};
			return this->invoke(seq, std::forward<Args2>(args)...);
		}

	private:
		
		template<std::size_t ...I, typename ...Args2>
		decltype(auto) invoke(std::index_sequence<I...>, Args2&&... args) const {
			return _internal(std::forward<Args>(std::get<I>(bindArgs)...)..., std::forward<Args2>(args)...);
		}

	private:
		std::tuple<Args&&...> bindArgs;
		F&& _internal;
	};

	template<typename Pointed, typename Class, typename This, typename ...Args>
	struct __binder_memfunc {

		__binder_memfunc(Pointed Class::* func, This&& _this, Args&&... args) : _internal(func), _this(std::forward<This>(_this)), bindArgs(std::forward<Args>(args)...) { 
			static_assert(std::is_same_v<Class, std::remove_pointer_t<std::decay_t<This>>>);
			static_assert(std::is_class_v<std::remove_pointer_t<std::decay_t<This>>>, "Binding a member function to a non");
		}

		template<typename ...Args2>
		decltype(auto) operator()(Args2&&... args) const {
			constexpr auto seq = std::make_index_sequence < std::tuple_size_v <std::tuple<Args...>>>{};
			return this->invoke(seq, std::forward<Args2>(args)...);
		}

	private:

		template<unsigned int ...I, typename ...Args2>
		decltype(auto) invoke(std::index_sequence<I...>, Args2&&... args) const {
			if constexpr (std::is_pointer_v<std::decay_t<This>>) {
				return (std::forward<This>(_this)->*_internal)(std::get<I>(bindArgs)..., std::forward<Args2>(args)...);
			}
			else if constexpr (std::is_reference_v<std::decay_t<This>> || std::is_object_v<std::decay_t<This>>) {
				return (std::foward(This)(_this).*_internal)(std::get<I>(bindArgs)..., std::forward<Args2>(args)...);
			}
		}

	private:
		Pointed Class::* _internal;
		This&& _this;
		std::tuple<Args&&...> bindArgs;
	};

	//effectively same as std::bind but leaves the rest of the arguments open
	template<typename F,typename ...Args>
	constexpr __binder<F, Args...>
	PartialBindFunction(F&& func, Args&&... args) {
		return __binder<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
	}

	template<typename Pointed, typename Class, typename This, typename ...Args>
	constexpr __binder_memfunc<Pointed, Class, This, Args...>
	PartialBindFunction(Pointed Class::* func, This&& _this, Args&&... args) {
		return __binder_memfunc(func, std::forward<This>(_this), std::forward<Args>(args)...);
	}
}