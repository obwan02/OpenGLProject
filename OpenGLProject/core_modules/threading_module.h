#pragma once
#include <iostream>
#include <functional>

#include "../module.h"
#include "../util/debug.h"

namespace ogl {

	class ThreadingModule : public Module, public RunnableModule {
		friend class Application;
		ThreadingModule() { }
	public:
		template<typename T, typename std::enable_if<std::is_base_of<RunnableModule, T>::value>::type* = nullptr>
		void RequestThread() {
			throw ErrNotImplemented();
		}

		template<typename T, typename std::enable_if<std::is_base_of<RunnableModule, T>::value>::type* = nullptr>
		void SpecifyMainThread() {
			// checks here if the module is loaded
			if (!m_RunnableModule.Set<T>()) {
				return;
			}
		}

	private:
		void Run() override {
			if (!m_RunnableModule.Complete()) {
				Debug::PrintLn("No main thread module specified for 'ThreadingModule'");
				return;
			}

			m_RunnableModule.reference->Run();
		}

	private:
		ModuleReference<RunnableModule> m_RunnableModule;
	};
}