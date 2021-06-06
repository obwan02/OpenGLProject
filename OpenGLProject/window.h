#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <shared_mutex>

namespace ogl {
	class Window {
	public:
		Window(const std::string& title, int width, int height, bool makeContextCurrent, bool* success);
		Window(const std::string& title, int width, int height, bool* success);
		Window(bool* success);
		/*static Window* CreateWindow(const char* title, int width, int height, bool makeContextCurrent);
		static Window* CreateWindow(const char* title, int width, int height);*/
		~Window();

		void MakeContextCurrent();
		bool ShouldClose();
		void SetShouldClose();

		void SetHeight(int height);
		void SetWidth(int width);
		void SetTitle(const std::string& title);

		void Update();

	private:
		GLFWwindow* m_GlfwWindow;

		static std::unordered_map<GLFWwindow*, Window*> s_Windows;
		static std::shared_mutex s_WindowsMutex;
	};
}