#include "application.h"

#include <sstream>

namespace ogl {

	Application* Application::s_Instance = nullptr;

	static void _glfwErrorCallback(int errorCode, const char* msg);
	static void _gladPostCallback(const char* name, void* funcPtr, int len_args, ...);

	Application::Application() : m_Running(false), m_Stopped(false) {
		OGL_ASSERT(s_Instance == nullptr);
		s_Instance = this;

		Init();
	}

	Application::~Application() {
		Stop();
		Destroy();
		s_Instance = nullptr;
	}

	Layer* Application::LoadLayer(Layer*&& layer) {
		m_Layers.push_back(std::unique_ptr<Layer>(layer));
		layer->OnAttached();
		return layer;
	}

	void Application::Run() {

		Time lastFrameTime = Time::GetTime();
		Time lastFPSDisplayTime = Time::GetTime();
		float numUpdates = 0;

		m_Running = true;
		while ((m_Running = !m_Window->GetShouldClose())) {

			static Time currentTime = Time::GetTime();
			static DeltaTime dt = currentTime - lastFrameTime;
			
			// process events
			m_Window->PollEvents();
			for (auto& ptr : m_Layers) {
				ptr->Update(dt);
			}
			m_Window->SwapBuffers();
			
			numUpdates++;
			if (currentTime - lastFrameTime >= 1.0f) {
				OGL_STATUS(numUpdates);
				lastFPSDisplayTime = currentTime;
			}

			lastFrameTime = currentTime;

		}
	}

	void Application::Stop() {
		OGL_INFO("Stopping Application");
		if (!m_Running) {
			OGL_INFO("Application is already being stopped or has stopped.");
			return;
		}

		m_Stopped = true;
		m_Running = false;
	}

	void Application::Init() {
		glfwSetErrorCallback((GLFWerrorfun)_glfwErrorCallback);
		if (glfwInit() == GLFW_FALSE) {
			OGL_ERROR("Failed to initialise GLFW");
		}
		m_Window = std::make_unique<Window>("Application Window", 1200, 600);
		m_Window->SetVSync(true);
#if defined(OGL_DEBUG) && defined(GLAD_DEBUG)
		glad_set_post_callback((GLADcallback) _gladPostCallback);
#endif
	}

	void Application::Destroy() {
		//GLFW
		m_Window.reset(nullptr); //delete window
		glfwTerminate();
	}

	static void _glfwErrorCallback(int errorCode, const char* msg) {
		OGL_ERROR(std::string("[GLFW ERROR] ") + msg);
	}

	static void _gladPostCallback(const char* name, void* funcPtr, int len_args, ...) {
		int error = glad_glGetError();
		if (error != GL_NO_ERROR) {
			std::stringstream s;
			s << "GL func \'" << name << "\' threw error code " << error;
			OGL_ERROR(s.str());
		}
	}
}
