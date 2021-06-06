#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <algorithm>
#include <array>
#include <unordered_set>

#include "module.h"
#include "util/uuid.h"
#include "util/debug.h"

#define APPLICATION_MODULE friend class ogl::Application;

namespace ogl {

	/*
		An application loads modules.
	*/
	class Application {
	public:
		Application();
		~Application();

		// Might make it so that modules cannot be loaded after start
		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr> void LoadModule();
		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr> T* const GetModule();
		template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type* = nullptr> bool IsModuleLoaded();
		// Don't use unless you know what you're doing. Most cases you should use ThreadModule::SpecifyMainThread.
		template<typename T, typename std::enable_if<std::is_base_of<RunnableModule, T>::value>::type* = nullptr> void SpecifyStartModule();
		// Gets modules that are derived from T.
		template<typename T> std::unordered_set<T*> GetModules();
		template<typename T> std::unordered_set<ModuleReference<T>> GetModuleReferences();

		bool IsModuleLoaded(const std::type_index&);
		Module* GetModule(const std::type_index&);

		void Run();
		void Stop();
		inline bool Running() { return m_Running; };
		
		static inline Application& Instance() { return *s_Instance; }
	private:
		std::unordered_map<std::type_index, std::unique_ptr<Module>> m_LoadedModules;
		ModuleReference<RunnableModule> m_StartModule;
		bool m_Running;
		bool m_Stopped;

		static Application* s_Instance;
	};



	template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type*> 
	void Application::LoadModule() {
		if (m_Running || m_Stopped) return;
		std::type_index index(typeid(T));
		if (!this->m_LoadedModules.count(index)) {
			std::unique_ptr<Module> ptr(static_cast<Module*>(new T));
			m_LoadedModules[index].swap(ptr);
			m_LoadedModules[index]->Attached();
		}
		else {
			// TODO: Debug Display: Module Already Loaded
		}
	}

	template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type*>
	T* const Application::GetModule() {
		std::type_index index(typeid(T));
		if (this->m_LoadedModules.count(index)) {
			return static_cast<T*>(this->m_LoadedModules[index].get());
		}

		return nullptr;
	}

	template<typename T, typename std::enable_if<std::is_base_of<Module, T>::value>::type*>
	bool ogl::Application::IsModuleLoaded()
	{
		std::type_index index(typeid(T));
		if (this->m_LoadedModules.count(index)) return true;
		return false;
	}

	template<typename T> 
	std::unordered_set<T*> Application::GetModules() {

		std::unordered_set<T*> v;
		// Dynamic cast will return nullptr for each module that doesn't implement the class
		for(auto& m : this->m_LoadedModules) {
			T* ptr = dynamic_cast<T*>(m.second.get());
			if (ptr != nullptr) {
				v.insert(ptr);
			}
		}

		return v;
	}

	template<typename T> 
	std::unordered_set<ModuleReference<T>> Application::GetModuleReferences() {
		std::unordered_set<ModuleReference<T>> v;
		// Dynamic cast will return nullptr for each module that doesn't implement the class
		for (auto& m : this->m_LoadedModules) {
			T* ptr = dynamic_cast<T*>(m.second.get());
			if (ptr != nullptr) {
				ModuleReference<T> dmr;
				auto p = std::make_unique<std::type_index>(m.first);
				dmr.index.swap(p);
				dmr.reference = ptr;
				v.insert(dmr);
			}
		}

		return v;
	}

	template<typename T, typename std::enable_if<std::is_base_of<RunnableModule, T>::value>::type*>
	void Application::SpecifyStartModule() {
		if (!this->IsModuleLoaded<T>()) {
			return;
		}

		m_StartModule.Set<T>();
	}
}