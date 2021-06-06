#pragma once
#include <functional>

namespace ogl {

	template<typename EventType, typename std::enable_if<std::is_base_of<Event, EventType>::value>::type* = nullptr>
	class EventDispatcher {

	};
}