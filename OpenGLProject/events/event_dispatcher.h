#pragma once
#include <oglpch.h>
#include "event.h"

namespace ogl {

	class EventDispatcher {
		EventDispatcher(const Event& e) : m_Event(e) { }

		template<typename ET, typename F, typename std::enable_if<std::is_base_of<Event, ET>::value>::type* = nullptr>
		void Dispatch(const std::function<void(const ET&)> func) {
			if (this->m_Event.GetStaticType() == ogl::GetEventType<ET>()) {
				const ET& e = &static_cast<ET&>(this->m_Event);
				func(e);
			}
		}

	private:
		const Event& m_Event;
	};
}