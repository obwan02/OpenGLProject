#pragma once
#include "vector.h"

namespace ogl {

	struct Transform2D {
		Vector3f position;
		float rotation;
		Vector2f scale;
	};
}