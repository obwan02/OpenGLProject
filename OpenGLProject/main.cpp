#include <chrono>

#include "application.h"
#include "module.h"
#include "core_modules/render_module.h"

int main() {
	using namespace ogl;
	Application myApplication;
	myApplication.LoadModule<RenderModule>();
	myApplication.Run();
}