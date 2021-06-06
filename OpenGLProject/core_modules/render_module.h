#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../module.h"
#include "../window.h"
#include "../application.h"
#include "../util/debug.h"

namespace ogl {

	void glfw_error_callback(int, const char* desc);

	class RenderModule : public Module, public UpdateableModule {
		friend class Application;
		RenderModule() { }
	public:

		void ApplicationStart();
		void Update() override;

		inline Window& GetWindow() {
			return *m_Window;
		}

	private:
		std::unique_ptr<Window> m_Window;
	};
}