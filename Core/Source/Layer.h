#pragma once
#include <memory>
#include "Event.h"

namespace Rune
{
#ifndef HEADLESS
	class Window;
#endif


	class Layer
	{
	public:
		virtual ~Layer() = default;
		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

		template<std::derived_from<Layer> T, typename... Args>
		void TransitionTo(Args&&... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}
		void TransitionTo(std::nullptr_t)
		{
			QueueTransition(nullptr);
		}

		// owner management (set by Window when pushing the layer)
		void SetOwner(Window* owner) { m_Owner = owner; }
		Window* GetOwner() const { return m_Owner; }

	private:
		void QueueTransition(std::unique_ptr<Layer> toLayer);

		Window* m_Owner = nullptr;
	};
}