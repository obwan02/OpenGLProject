#include "application.h"
#include "graphics/2D/renderer2d.h"
#include "graphics/texture.h"
#include "log.h"
#include "math/vector.h"
#include <memory>

namespace ogl {

	static void _glfwErrorCallback(int errorCode, const char* msg);

	Application::Application() : m_Window(nullptr) {}
	Application::~Application() {
		m_Window.reset(nullptr);
		glfwTerminate();
	}

	void Application::init() {
		// TODO: Move this to better place
		std::ios::sync_with_stdio(false);

		// Init GLFW
		glfwSetErrorCallback(_glfwErrorCallback);
		if(!glfwInit()) {
			ogl::log::CriticalFrom("GLFW", "Failed to initialise GLFW");
			OGL_ASSERT(false);
		}
		ogl::log::InfoFrom("GLFW", "GLFW Version: ", glfwGetVersionString());

		// Window creation	
		auto win = Window::Create("Pee pee poo poo", 1920, 1080);
		OGL_ASSERT(win, "Failed to create window :(");
		m_Window = std::make_unique<Window>(std::move(*win));
		m_Window->set_vsync(true);

		// Print GL version
		ogl::log::InfoFrom("GL", "GL Version ", m_Window->context().GL.majorVersion, '.', m_Window->context().GL.minorVersion);
	}

	void Application::run() {
		Renderer2D renderer(m_Window->context());


		auto preImage = Image::Open("./num2.png");
		OGL_ASSERT(preImage, "Failed to open ./num2.png");
		const auto image = std::move(*preImage);

		Vector3f pos{0, 0, 0};
		Vector2f size{100, 100};
		Vector4f colour{1, 1, 1, 1};
		TexCoords coords{{0, 0}, {1, 1}};
		auto texture = std::make_shared<Texture2D>(image, true);

		Renderer2DSpriteData renderData(&pos, &size, &colour, &coords, texture.get(), 1);

		Vector2f velocity{ 2, 2 };
		while(!m_Window->get_should_close()) {
			const auto& context = m_Window->context();
			if (pos.x + size.x > context.frameBufferWidth / 2) velocity.x *= -1;
			if (pos.x < -context.frameBufferWidth / 2) velocity.x *= -1;
			if (pos.y + size.y > context.frameBufferHeight / 2) velocity.y *= -1;
			if (pos.y < -context.frameBufferHeight / 2) velocity.y *= -1;

			pos += Vector3f{ velocity.x, velocity.y, 0.0 };

			renderer.process_dynamic(renderData, m_Window->context());
			renderer.flush(m_Window->context());
			m_Window->poll_events();
			m_Window->swap_buffers();
		}
	}

	static void _glfwErrorCallback(int errorCode, const char* msg) {
		ogl::log::ErrorFrom("GLFW", msg);
	}

}
