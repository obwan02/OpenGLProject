#pragma once
#include <memory>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>
#include <optional>

#include "core.h"
#include "graphics/context.h"

namespace ogl {
	class Window {
		friend class Application;
	public:
		// Don't call this. It is for internal use only. Please
		// create a window using Window::Create. This constructor is only private
		// because std::make_optional has to work.
		Window(GLFWwindow* window, const GraphicsContext&);
		Window(Window&& other);
		Window(const Window&) = delete;
		~Window();

		void set_should_close();
		void set_width(int width);
		void set_height(int height);
		void set_title(const std::string& title);
		void set_vsync(bool enable);
		
		bool get_should_close();
		int get_width();
		int get_height();
		int get_framebuffer_width();
		int get_framebuffer_height();

		void make_context_current();
		void poll_events();
		void swap_buffers();

		const GraphicsContext& context() { return m_Context; }
		GLFWwindow* get_glfw_pointer() { return m_GlfwWindow; };

		static std::optional<Window> Create(const std::string& title, int width = 100, int height = 100);

	private:
		static Window& window_from_glfw(GLFWwindow* window) { 
			OGL_DEBUG_ASSERT(s_Windows.find(window) != s_Windows.end(), "GLFWWindow* doesn't exist in in hash map");
			return *s_Windows[window]; 
		} 

		friend void _glfwWindowCloseCallback(GLFWwindow*);
		friend void _glfwFramebufferResizeCallback(GLFWwindow*, int, int);
		friend void _glfwKeyCallback(GLFWwindow*, int, int, int, int);
	private:
		GLFWwindow* m_GlfwWindow;
		GraphicsContext m_Context; 

		// no possibility of use after free because lifetime of GLFWwindow is tied to Window
		static std::unordered_map<GLFWwindow*, Window*> s_Windows;
		static bool s_GladInitialised;
	};
}
