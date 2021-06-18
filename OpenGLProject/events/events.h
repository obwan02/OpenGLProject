#pragma once
#include "event.h"

namespace ogl {

	class KeyDownEvent : public Event {
	public:
		KeyDownEvent(int keyCode) : keyCode(keyCode) { }

		void GetStaticType() override {
			return GetEventType<KeyDownEvent>();
		}
	};

	int keyCode;
}