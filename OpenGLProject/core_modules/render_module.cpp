#include "render_module.h"
#include "../util/debug.h"

namespace ogl {
	void glfw_error_callback(int error_code, const char* desc) {
		Debug::Print("[GLFW ERROR] Code: ");
		Debug::Print(std::to_string(error_code));
		Debug::PrintLn(std::string("; [Description] ") + desc);
	}

	void glad_pre_callback(const char* name, void* funcptr, int len_args, ...) {
		Debug::PrintGLVerboseLn("About to call GL func: \"" + std::string(name) + "\"");
	}

	void glad_post_callback(const char* name, void* funcptr, int len_args, ...) {
		std::string funcName(name);
		Debug::PrintGLVerboseLn("Called GL func: \"" + funcName  + "\"");
		GLenum errorCode = glad_glGetError();

		if(errorCode != GL_NO_ERROR) {
			Debug::PrintErrorLn("Error in GL func \"" + funcName + "\"");
		}
	}

	void RenderModule::ApplicationStart() {
		if (glfwInit() == GLFW_FALSE) {
			Debug::PrintLn("Failed to initialise glfw.");
			Application::Instance().Stop();
		}

		glfwSetErrorCallback(glfw_error_callback);

		bool loaded;
		m_Window.reset(new Window(&loaded));
		if (!loaded) {
			Debug::PrintLn("Failed to create window/make context current.");
			Application::Instance().Stop();
		}

		m_Window->MakeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			Debug::PrintLn("Failed to load proc address.");
			Application::Instance().Stop();
		}
	}

	void RenderModule::Update() {
		if (m_Window->ShouldClose()) {
			Application::Instance().Stop();
			return;
		}
		//TODO: render stuff here
		m_Window->Update();
	}
}