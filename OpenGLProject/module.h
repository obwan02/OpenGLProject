#pragma once
#include <functional>
#include <shared_mutex>
#include <set>
#include <typeindex>
#include "util/uuid.h"
//#include "events/event.h"

namespace ogl {

	class Module {
	public:
		Module(const Module&) = delete; // delete copy constructor
		Module(Module&&) = delete; // delete move constructor
		Module& operator=(const Module&) = delete; // delete assignment operator;

		virtual void Attached() { };
		virtual void Dettached() { };
		virtual void ApplicationStart() { };
		virtual void ApplicationStop() { };

	protected:
		Module() = default; // default constructor is protected so we can inherit
	private:
		friend class Application;
	};

	class UpdateableModule {
	public:
		virtual int UpdateOrder() { return 0; };
		virtual void Update() {}
		friend class Application;
	};
	
	class RunnableModule {
	public:
		virtual void Run() = 0;
		friend class Application;
	};

	template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr>
	inline std::type_index ModuleId() {
		return std::type_index(typeid(T));
	}

	template<typename Base = Module>
	struct ModuleReference {

		ModuleReference() { index.reset(nullptr); reference = nullptr; }
		ModuleReference(const ModuleReference& other) {
			auto ptr = std::make_unique<std::type_index>(*other.index);
			index.swap(ptr);

			reference = other.reference;
		} // copy constructor

		template<typename Derived, typename std::enable_if<std::is_base_of<Base, Derived>::value>::type* = nullptr, typename std::enable_if<std::is_base_of<Module, Derived>::value>::type* = nullptr>
		bool Set() {
			auto ptr = std::make_unique<std::type_index>(typeid(Derived));
			index.swap(ptr);

			if (!this->IsDerivedLoaded()) {
				index.reset(nullptr);
				return false;
			}

			reference = static_cast<Base*>(Application::Instance().GetModule<Derived>());
			return true;
		}

		void Clear() {
			index.reset(nullptr);
			reference = nullptr;
		}

		bool IsDerivedLoaded() const {
			if(index == nullptr) return false;
			return Application::Instance().IsModuleLoaded(*index);
		}

		bool Complete() const {
			return reference != nullptr && index != nullptr;
		}

		size_t hash_code() const {
			return index->hash_code();
		}

		bool operator==(const ModuleReference<Base>& other) const {
			if (other.index == nullptr || index == nullptr) {
				return other.index == index; // equal if both are nullptr
			}
			return *index == *other.index && reference == other.reference;
		}

		std::unique_ptr<std::type_index> index;
		Base* reference;
	};
}

namespace std {
	template<typename T>
	struct hash<ogl::ModuleReference<T>> {
		size_t operator()(const ogl::ModuleReference<T>& _Keyval) const noexcept {
			return _Keyval.hash_code();
		}
	};

	template<typename T>
	struct equal_to<ogl::ModuleReference<T>> {
		constexpr bool operator()(const ogl::ModuleReference<T>& left, const ogl::ModuleReference<T>& right) const noexcept {
			return left == right;
		}
	};
}
