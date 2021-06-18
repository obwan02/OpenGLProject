#pragma once
#include <oglpch.h>
#include "util/util.h"
#include "events/event.h"

namespace ogl {
	class Window {
	public:
		Window(const std::string& title, int width = 100, int height = 100);
		~Window();

		void SetShouldClose();
		void SetWidth(int width);
		void SetHeight(int height);
		void SetTitle(const std::string& title);
		void SetVSync(bool enable);

		template<typename F>
		void SetEventCallback(const F& func) { this->m_EventCallback = func; }
		
		bool GetShouldClose();
		int GetWidth();
		int GetHeight();
		int GetFrameBufferWidth();
		int GetFrameBufferHeight();

		void MakeContextCurrent();
		void PollEvents();
		void SwapBuffers();

		inline GLFWwindow* GetGLFWPointer() { return m_GlfwWindow; };

		static inline Window& GetWindowFromGLFWPointer(GLFWwindow* window) { return *s_Windows[window]; } // no error check
	private:
		GLFWwindow* m_GlfwWindow;
		std::function<void(Event&)> m_EventCallback;

		static bool s_GladInitialised;
		static std::unordered_map<GLFWwindow*, Window*> s_Windows; // no possibility of use after free because lifetime of GLFWwindow is tied to Window
	};
}