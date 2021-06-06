#include "window.h"

namespace ogl {

	std::unordered_map<GLFWwindow*, Window*> Window::s_Windows = std::unordered_map<GLFWwindow*, Window*>();
	std::shared_mutex Window::s_WindowsMutex;

	static void _glfwWindowResizeCallback(GLFWwindow* window, int width, int height);
	static void _glfwWindowCloseCallback(GLFWwindow* window);

	Window::Window(const std::string& title, int width, int height, bool makeContextCurrent, bool* success) : m_GlfwWindow(nullptr) {
		m_GlfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (m_GlfwWindow == NULL) {
			if(success != nullptr) *success = false;
			return;
		}

		//GLFW callbacks
		glfwSetWindowSizeCallback(m_GlfwWindow, (GLFWwindowsizefun)_glfwWindowResizeCallback);
		glfwSetWindowCloseCallback(m_GlfwWindow, (GLFWwindowclosefun)_glfwWindowCloseCallback);

		if(makeContextCurrent) glfwMakeContextCurrent(m_GlfwWindow);
		s_WindowsMutex.lock();
		s_Windows[m_GlfwWindow] = this;
		s_WindowsMutex.unlock();
		*success = true;
	}

	Window::Window(const std::string& title, int width, int height, bool* success) : Window::Window(title, width, height, true, success) { }

	Window::Window(bool* success) : Window::Window("OGL DEFAULT WINDOW", 200, 200, true, success) {}

	Window::~Window() {
		glfwDestroyWindow(m_GlfwWindow);
		s_WindowsMutex.lock();
		s_Windows.erase(m_GlfwWindow);
		s_WindowsMutex.unlock();
	}

	void Window::MakeContextCurrent() {
		glfwMakeContextCurrent(m_GlfwWindow);
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(m_GlfwWindow);
		
	}

	void Window::SetShouldClose() {
		glfwSetWindowShouldClose(m_GlfwWindow, GLFW_TRUE);
	}

	void Window::SetWidth(int width) {
		int height;
		glfwGetWindowSize(m_GlfwWindow, nullptr, &height);
		glfwSetWindowSize(m_GlfwWindow, width, height);
	}

	void Window::SetTitle(const std::string& title) {
		const char* t = title.c_str();
		glfwSetWindowTitle(m_GlfwWindow, t);
	}

	void Window::Update() {
		glfwSwapBuffers(m_GlfwWindow);
		glfwPollEvents();
	}

	void Window::SetHeight(int height) {
		int width;
		glfwGetWindowSize(m_GlfwWindow, &width, nullptr);
		glfwSetWindowSize(m_GlfwWindow, width, height);
	}

	static void _glfwWindowResizeCallback(GLFWwindow* window, int width, int height) {
		auto _cc = glfwGetCurrentContext();
		if(_cc != window) glfwMakeContextCurrent(window);
		glViewport(0, 0, width, height);
		if (_cc != window) glfwMakeContextCurrent(_cc);
	}

	static void _glfwWindowCloseCallback(GLFWwindow* window) {

	}

	
}
