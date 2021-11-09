#pragma once

#include <memory>
#include <optional>
#include <sstream>

#include "core.h"
#include "graphics/context.h"
#include "window.h"
#include "util/time.h"

namespace ogl {

	class Application {
	public:
		Application();
		~Application();
		void init();
		void run();
	private:
		std::unique_ptr<Window> m_Window;
	};
};
