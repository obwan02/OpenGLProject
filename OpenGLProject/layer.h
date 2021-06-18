#pragma once
#include <oglpch.h>
#include "events/event.h"
#include "util/debug.h"
#include "util/time.h"

namespace ogl {

	class Layer {
	public:
		Layer(const Layer&) = delete; // delete copy constructor
		Layer(Layer&&) = delete; // delete move constructor
		Layer& operator=(const Layer&) = delete; // delete assignment operator;

		virtual ~Layer() { };

		virtual void OnAttached() { };
		virtual void OnDettached() { };
		virtual void PreUpdate(DeltaTime deltaTime) { };
		virtual void Update(DeltaTime deltaTime) { };
		virtual void PostUpdate(DeltaTime deltaTime) { };
		virtual void OnEvent(Event* e) { };
		virtual int GetLayerIndex() { return 0; };

		std::vector<std::unique_ptr<Layer>> children;

	protected:
		Layer() = default; // default constructor is protected so we can inherit
	private:
		friend class Application;
	};
}
