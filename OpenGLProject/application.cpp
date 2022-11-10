#include "application.h"
#include "graphics/2D/instance_renderer.h"
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
		BatchRenderer2D renderer(m_Window->context());

		auto preImage = Image::open("./num2.png");
		OGL_ASSERT(preImage, "Failed to open ./num2.png");
		const auto image = std::move(*preImage);

		Vector3f positions[1000];
		Vector2f sizes[1000];
		Vector2f vels[1000];
		Vector4f colours[1000];
		TexCoords coordss[1000];
		std::shared_ptr<Texture2D> textures[1000];
		auto texture = std::make_shared<Texture2D>(image, true);
		
		auto gen_rand = [=](auto i) { return (float)(rand() % i); };

		for (int i = 0; i < 1000; i++) {
			sizes[i] = Vector2f{100, 100};
			positions[i] = Vector3f{gen_rand(1820) - 960, gen_rand(1820) - 960, 0};
			colours[i] = Vector4f{ 1, 1, 1, 1 };
			coordss[i] = TexCoords{{0, 0}, {1, 1}};
			textures[i] = texture;
			vels[i] = Vector2f{ gen_rand(3)*2 - 3, gen_rand(3) - 3 };
		}

		RendererSpriteData renderData(positions, sizes, colours, coordss, textures, 1000);

		Vector2f velocity{ 2, 2 };
		while(!m_Window->get_should_close()) {
			const auto& context = m_Window->context();

			for (int i = 0; i < 1000; i++) {
				auto& pos = positions[i];
				auto& size = sizes[i];
				auto& velocity = vels[i];
				if (pos.x + size.x > context.frameBufferWidth / 2) velocity.x *= -1;
				if (pos.x < -context.frameBufferWidth / 2) velocity.x *= -1;
				if (pos.y + size.y > context.frameBufferHeight / 2) velocity.y *= -1;
				if (pos.y < -context.frameBufferHeight / 2) velocity.y *= -1;

				pos.x += velocity.x;
				pos.y += velocity.y;
			}


			renderer.process(renderData, m_Window->context());
			renderer.flush(m_Window->context());
			m_Window->poll_events();
			m_Window->swap_buffers();
		}
	}

	static void _glfwErrorCallback(int errorCode, const char* msg) {
		ogl::log::ErrorFrom("GLFW", msg);
	}

}
