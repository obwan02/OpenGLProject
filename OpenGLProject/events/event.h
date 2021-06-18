#pragma once
#include <oglpch.h>

namespace ogl {

	typedef std::type_index EventType;

	class Event {
	public:
		virtual EventType GetStaticType() = 0;
	};

	template<typename T>
	EventType GetEventType() {
		return std::type_index(typeid(T));
	}
}