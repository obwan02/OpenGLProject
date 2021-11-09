#include "application.h"
#include "graphics/2D/renderer2d.h"
#include "util/storage.h"
#include "math/vector.h"

template <typename T, typename U>
struct decay_equiv :
	std::is_same<typename std::decay<T>::type, U>::type
{};

int main() {

	// TODO: Change to passing parameters around:
	// - Instead of ogl::GraphicsInfo we would just have 
	// a GraphicsContext variable that is passed around.
	// - Find other places to implement this.
	// - More procedural

	ogl::Application app;
	app.init();
	app.run();
}
