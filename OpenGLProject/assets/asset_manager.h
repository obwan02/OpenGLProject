#pragma once

#include <optional>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <any>
#include <utility>

#include "assert.h"
#include "asset.h"
#include "util/greedy_vector.h"
#include "core.h"


namespace ogl {
	

	namespace intern {
		int global_index() {
			static int i = 0;
			return i++;
		}

		template<typename T>
		int type_index() {
			static int index = global_index();
			return index;
		}
	}
	
	class AssetManager;

	template<typename T>
	class AssetHandle {
		friend AssetManager;
	private:
		AssetHandle(std::string name);

	public:
		inline T* get_ptr();
		inline T& get() { return *get_ptr(); }
		inline std::string name() { return m_Name;	}


	private:
		std::string m_Name;
		T* m_Data;
	};

	class AssetManager {
	private:
		AssetManager() : m_AssetMap(100) { }
	
	public:
		template<typename T>
		std::optional<AssetHandle<T>> create(const std::string& name, const std::string& path) {	
			static_assert(std::is_invocable_r<T, decltype(T::construct_asset), const std::string&>::value, "T must have a static function 'T::construct_asset(const std::string&)' that returns a valid std::optional<T>");
			static_assert(std::is_move_constructible_v<T>, "T must be move constructible");
			if(T data = T::construct_asset(path)) {
				T* ptr = new T(std::move(data));
				auto key = std::make_pair(std::string{name}, intern::type_index<T>());
				m_AssetMap[key] = ptr;
				OGL_ASSERT(false, "TODO implement asset handle");
				return std::make_optional(AssetHandle<T>(std::string{name}));
			}
		}

		template<typename T>
		std::optional<T&> get(const std::string& name) {
			auto key = std::make_pair(std::string{name}, intern::type_index<T>());
			
			// If not found
			if(m_AssetMap.find(key) == m_AssetMap.end()) {
				return std::nullopt;
			}

			return *((T*) m_AssetMap[key]);
		}

		template<typename T> 
		bool free(const std::string& name) {
			auto key = std::make_pair(std::string{name}, intern::type_index<T>());

			if(m_AssetMap.find(key) == m_AssetMap.end()) {
				return false;
			}

			delete ((T*) m_AssetMap[key]);
			m_AssetMap.erase(key);
		}

		static AssetManager& instance() {
			static AssetManager inst{};
			return inst;
		}

	private:
		std::unordered_map<std::pair<std::string, int>, void*> m_AssetMap;
	};

	template<typename T>
	T* AssetHandle<T>::get_ptr() {
		OGL_DEBUG_ASSERT(AssetManager::instance().get(this->name).has_value());
		return m_Data; 
	}
}
