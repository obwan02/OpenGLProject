#include "window.h"
#include "GLFW/glfw3.h"
#include "core.h"
#include "debug.h"
#include "graphics/context.h"
#include "log.h"
#include <memory>
#include <optional>
#include <sstream>

namespace ogl {

	void MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		const char* sourceMsg;
		switch (source) {
			case GL_DEBUG_SOURCE_API:             sourceMsg = "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceMsg = "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceMsg =  "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceMsg =  "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     sourceMsg =  "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           sourceMsg =  "Source: Other"; break;
			default: sourceMsg = "Source: ???";
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			ogl::log::ErrorFrom("GL Debug", "<", sourceMsg, "> ",  message);
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		case GL_DEBUG_TYPE_PERFORMANCE:
			ogl::log::WarnFrom("GL Debug", "<", sourceMsg, "> ",  message);
			break;
		}
	}

	bool Window::s_GladInitialised = false;
	std::unordered_map<GLFWwindow*, Window*> Window::s_Windows = std::unordered_map<GLFWwindow*, Window*>();

	void _glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height);
	void _glfwWindowCloseCallback(GLFWwindow* window);
	void _glfwKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void _gladPostCallback(const char* name, void* funcPtr, int len_args, ...);

	// Create will create a window. Returns nullopt if the window failed to create.
	// Automatically will initialise GLAD if it has not been initialised yet.
	std::optional<Window> Window::Create(const std::string& title, int width, int height) {

		const char* t = title.c_str();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OGL_TARGET_GL_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OGL_TARGET_GL_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(OGL_DEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		const auto glfwWin = glfwCreateWindow(width, height, t, NULL, NULL);

		if(glfwWin == NULL) {
			log::Critical("Failed to create GLFW window.");
			return std::nullopt;
		}

		//GLFW callbacks
		glfwSetFramebufferSizeCallback(glfwWin, (GLFWwindowsizefun)_glfwFramebufferResizeCallback);
		glfwSetWindowCloseCallback(glfwWin, (GLFWwindowclosefun)_glfwWindowCloseCallback);
		glfwSetKeyCallback(glfwWin, (GLFWkeyfun)_glfwKeyCallback);
		
		// we make context current here instead of in the if block for consistency 
		glfwMakeContextCurrent(glfwWin); 
		if (!s_GladInitialised) {
			if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				log::Critical("Failed to initialise GLAD.");
				return std::nullopt;
			}
			s_GladInitialised = true;
		}

#if defined(OGL_DEBUG) && defined(GLAD_DEBUG)
#if defined(GLAD_DEBUG)
		glad_set_post_callback((GLADcallback)_gladPostCallback);
#endif
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
#endif
		glEnable(GL_DEPTH_TEST);
	
		// Init graphics context
		int major, minor, slots, fragSlots;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &slots);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &fragSlots);
		// TODO: query glfw for window size instead of using params
		
		// Please god let C++20 become more common sooner
		GraphicsContext context{
			/* .GL = */ { /* .majorVersion = */ major, /* .minorVersion = */ minor },
			/* .maxTotalTextureSlots = */ slots,
			/* .maxFragmentTextureSlots = */ fragSlots,
			/* .frameBufferWidth = */ width,
			/* .frameBufferHeight = */ height
		};

		return std::make_optional<Window>(glfwWin, context);
	}

	Window::Window(GLFWwindow* window, const GraphicsContext& context) : m_Context(context) {
		m_GlfwWindow = window;
		s_Windows[window] = this; 
	}

	Window::Window(Window&& other) : m_GlfwWindow(other.m_GlfwWindow), m_Context(other.m_Context) {
		other.m_GlfwWindow = nullptr;
		s_Windows[m_GlfwWindow] = this;
	}

	Window::~Window() {
		if(m_GlfwWindow != nullptr) {
			glfwDestroyWindow(m_GlfwWindow);
			s_Windows.erase(m_GlfwWindow);
		}
	}

	void Window::set_should_close() {
		glfwSetWindowShouldClose(m_GlfwWindow, GLFW_TRUE);
	}

	void Window::set_width(int width) {
		int height;
		glfwGetWindowSize(m_GlfwWindow, nullptr, &height);
		glfwSetWindowSize(m_GlfwWindow, width, height);
	}

	void Window::set_height(int height) {
		int width;
		glfwGetWindowSize(m_GlfwWindow, &width, nullptr);
		glfwSetWindowSize(m_GlfwWindow, width, height);
	}

	void Window::set_title(const std::string& title) {
		const char* t = title.c_str();
		glfwSetWindowTitle(m_GlfwWindow, t);
	}

	void Window::set_vsync(bool enable) {
		glfwSwapInterval((int)enable);
	}

	bool Window::get_should_close() {
		return glfwWindowShouldClose(m_GlfwWindow);
	}

	int Window::get_width() {
		int width;
		glfwGetWindowSize(m_GlfwWindow, &width, nullptr);
		return width;
	}

	int Window::get_height() {
		int height;
		glfwGetWindowSize(m_GlfwWindow, nullptr, &height);
		return height;
	}

	int Window::get_framebuffer_width() {
		int width;
		glfwGetFramebufferSize(m_GlfwWindow, &width, nullptr);
		return width;
	}

	int Window::get_framebuffer_height() {
		int height;
		glfwGetFramebufferSize(m_GlfwWindow, nullptr, &height);
		return height;
	}

	void Window::make_context_current() {
		glfwMakeContextCurrent(m_GlfwWindow);
	}

	void Window::poll_events() {
		glfwPollEvents();
	}

	void Window::swap_buffers() {
		glfwSwapBuffers(m_GlfwWindow);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	

	void _glfwWindowCloseCallback(GLFWwindow* window) {
		ogl::log::InfoFrom("GLFW", "Closing window");
	}

	void _glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		auto &win = Window::window_from_glfw(window);
		win.m_Context.frameBufferWidth = width;
		win.m_Context.frameBufferHeight = height;
	}

	void _glfwKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
		ogl::log::InfoFrom("GLFW", "Key pressed!");
	}

	static void _gladPostCallback(const char* name, void* funcPtr, int len_args, ...) {
		bool hasErrored = false;
		std::stringstream s;
		
		int error;
		while((error = glad_glGetError()) != GL_NO_ERROR) {
			s << "GL func \'" << name << "\' threw error code " << error << '\n';
			hasErrored = true;
		}

		if (hasErrored) ogl::log::Error(s.str());
	}
}
