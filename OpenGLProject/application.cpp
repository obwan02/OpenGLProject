#include "application.h"
#include "core_modules/threading_module.h"
#include "core_modules/update_module.h"

namespace ogl {

	Application* Application::s_Instance = nullptr;

	void Application::Run() {

		if (m_Stopped) return;

		if (!m_StartModule.Complete()) {
			Debug::PrintLn("No 'RunnableModule' was specified to start the application");
			throw std::logic_error("No 'RunnableModule' was specified to start the application");
			//TODO: Might change to debug print and just return or exit(-1).
			// or just return false
		}

		Module* startModuleAsModule = GetModule(*m_StartModule.index);

		m_Running = true;
		for(auto& m : m_LoadedModules) {
			m.second->ApplicationStart();
			// we need to check if application has aborted operation by calling stop
			if (!m_Running) {
				return;
			}
		}

		m_StartModule.reference->Run();
	}

	void Application::Stop() {
		if (!m_Running) return;
		m_Stopped = true;
		m_Running = false;
		Module* startModuleAsModule = GetModule(*m_StartModule.index);
		RunnableModule* startModule = m_StartModule.reference;

		for (auto& m : m_LoadedModules) {
			m.second->ApplicationStop();
		}
	}

	bool Application::IsModuleLoaded(const std::type_index& index) {
		if(m_LoadedModules.count(index)) return true;
		return false;
	}

	Module* ogl::Application::GetModule(const std::type_index& index) {
		if (m_LoadedModules.count(index)) {
			return m_LoadedModules[index].get();
		}

		return nullptr;
	}

	Application::Application() : m_Running(false), m_Stopped(false) {
		s_Instance = this;
		m_StartModule.Clear();
#ifndef NO_AUTO_LOAD_THREADING_MODULE
		LoadModule<ThreadingModule>();
#endif
#ifndef NO_AUTO_LOAD_UPDATE_MODULE
		LoadModule<UpdateModule>();
#endif
#ifndef NO_AUTO_LOAD_THREADING_MODULE
#ifndef NO_AUTO_LOAD_UPDATE_MODULE
		SpecifyStartModule<ThreadingModule>();
		GetModule<ThreadingModule>()->SpecifyMainThread<UpdateModule>();
#endif
#endif
	}

	Application::~Application() {
		Stop();
		s_Instance = nullptr;
	}
}
