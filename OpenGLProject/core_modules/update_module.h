#pragma once
#include <unordered_set>
#include <GLFW/glfw3.h>

#include "../application.h"
#include "../module.h"

namespace ogl {

	class UpdateModule : public Module, public RunnableModule {
		friend Application;
		UpdateModule() {};
	public:

		void Attached() override {
			m_Fps = 60;
		}

		void ApplicationStart() override {

			// Add all UpdateableModule modules.
			auto newUpdateables = Application::Instance().GetModuleReferences<UpdateableModule>();
			m_Updateables.insert(newUpdateables.begin(), newUpdateables.end());

			m_ShouldUpdate = true;
		}

		void ApplicationStop() override {
			m_ShouldUpdate = false;
		}

		template<typename T, typename std::enable_if<std::is_base_of<UpdateableModule, T>::value>::type* = nullptr>
		void RegisterNewUpdateable() {
			ModuleReference<UpdateableModule> dmr;
			if(dmr.Set<T>()) m_Updateables.push_back(dmr);
		}

		void Run() {
			char debugMsg[20];
			float prev = (float)glfwGetTime();
			float fps_prev = (float)glfwGetTime();


			while (m_ShouldUpdate) {
				float target = 1.0f/m_Fps;
				float now = (float)glfwGetTime();
				float dt = now - prev;
				if (dt >= target) {
					m_DeltaTime = now - prev;
					prev = now;
					// update modules
					for (auto iter = m_Updateables.begin(); iter != m_Updateables.end();) {
						auto dmr = *iter;
						if (dmr.IsDerivedLoaded()) {
							dmr.reference->Update();
							++iter;
						}
						else {
							iter = m_Updateables.erase(iter);
						}
					}
				}

				//debug display
				dt = now - fps_prev;
				if(dt >= 1.0f) {
					fps_prev = (float)glfwGetTime();
					sprintf_s(debugMsg, "FPS: %.2f", 1.0f/DeltaTime());
					Debug::PrintLn(debugMsg);
				}
			
			}
		}

		float DeltaTime() {
			return m_DeltaTime;
		}
		

	private:
		float m_Fps;
		float m_DeltaTime;
		bool m_ShouldUpdate;
		std::unordered_set<ModuleReference<UpdateableModule>> m_Updateables;
	};
}