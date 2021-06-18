#pragma once
#include <oglpch.h>
#include "layer.h"
#include "window.h"
#include "util/time.h"

namespace ogl {

	/*
		An application loads modules.
	*/
	class Application {
		friend Layer;
	public:
		Application();
		~Application();

		Layer* LoadLayer(Layer*&& layer); // passed as an rvalue reference so only we control it

		void Run();
		void Stop();

		inline bool Running() { return m_Running; };
		static inline Application& Instance() { return *s_Instance; }

	protected:
		void Init();
		void Destroy();
	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		bool m_Stopped;

		static Application* s_Instance;
	};
}