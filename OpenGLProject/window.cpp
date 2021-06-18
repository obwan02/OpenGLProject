#include "window.h"

namespace ogl {

	bool Window::s_GladInitialised = false;
	std::unordered_map<GLFWwindow*, Window*> Window::s_Windows = std::unordered_map<GLFWwindow*, Window*>();

	static void _glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void _glfwWindowCloseCallback(GLFWwindow* window);
	static void _glfwKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	Window::Window(const std::string& title, int width, int height) {
		const char* t = title.c_str();
		m_GlfwWindow = glfwCreateWindow(width, height, t, NULL, NULL);

		OGL_ASSERT(m_GlfwWindow != NULL);

		//GLFW callbacks
		glfwSetFramebufferSizeCallback(m_GlfwWindow, (GLFWwindowsizefun)_glfwFramebufferResizeCallback);
		glfwSetWindowCloseCallback(m_GlfwWindow, (GLFWwindowclosefun)_glfwWindowCloseCallback);
		glfwSetKeyCallback(m_GlfwWindow, (GLFWkeyfun)_glfwKeyCallback);

		MakeContextCurrent(); // we make context current here instead of in the if block for consistency 
		if (!s_GladInitialised) {
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		}

		s_Windows[m_GlfwWindow] = this;
	}

	Window::~Window() {
		glfwDestroyWindow(m_GlfwWindow);
		s_Windows.erase(m_GlfwWindow);
	}

	void Window::SetShouldClose() {
		glfwSetWindowShouldClose(m_GlfwWindow, GLFW_TRUE);
	}

	void Window::SetWidth(int width) {
		int height;
		glfwGetWindowSize(m_GlfwWindow, nullptr, &height);
		glfwSetWindowSize(m_GlfwWindow, width, height);
	}

	void Window::SetHeight(int height) {
		int width;
		glfwGetWindowSize(m_GlfwWindow, &width, nullptr);
		glfwSetWindowSize(m_GlfwWindow, width, height);
	}

	void Window::SetTitle(const std::string& title) {
		const char* t = title.c_str();
		glfwSetWindowTitle(m_GlfwWindow, t);
	}

	void Window::SetVSync(bool enable) {
		glfwSwapInterval((int)enable);
	}

	bool Window::GetShouldClose() {
		return glfwWindowShouldClose(m_GlfwWindow);
	}

	int Window::GetWidth() {
		int width;
		glfwGetWindowSize(m_GlfwWindow, &width, nullptr);
		return width;
	}

	int Window::GetHeight() {
		int height;
		glfwGetWindowSize(m_GlfwWindow, nullptr, &height);
		return height;
	}

	int Window::GetFrameBufferWidth() {
		int width;
		glfwGetFramebufferSize(m_GlfwWindow, &width, nullptr);
		return width;
	}

	int Window::GetFrameBufferHeight() {
		int height;
		glfwGetFramebufferSize(m_GlfwWindow, nullptr, &height);
		return height;
	}

	void Window::MakeContextCurrent() {
		glfwMakeContextCurrent(m_GlfwWindow);
	}

	void Window::PollEvents() {
		glfwPollEvents();
	}

	void Window::SwapBuffers() {
		glfwSwapBuffers(m_GlfwWindow);
	}

	static void _glfwWindowCloseCallback(GLFWwindow* window) {

	}

	static void _glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	static void _glfwKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
		OGL_DEBUG_INFO("Key Event");
		ogl::Window& w = ogl::Window::GetWindowFromGLFWPointer(window);
		
	}

	
}
