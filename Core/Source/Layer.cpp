#include "Layer.h"
#include "Application.h"
namespace Rune
{
	void Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
	{

		// If this layer has an owner window, replace in that window's stack
		if (m_Owner)
		{
			auto& layerStack = m_Owner->m_LayerStack; // Layer is friend of Window
			for (auto& layer : layerStack)
			{
				if (layer.get() == this)
				{
					if (toLayer)
						toLayer->SetOwner(m_Owner);
					layer = std::move(toLayer);
					return;
				}
			}
		}

		//TODO : this is a bit hacky, but it works for now. We can optimize this later by keeping track of the current layer index in the application.
		auto& layerStack = Window::Get().m_LayerStack;
		for (auto& layer : layerStack)
		{
			if (layer.get() == this)
			{
				layer = std::move(toLayer);
				return;
			}
		}
		assert(false && "Layer::QueueTransition: layer not found in owner or application stacks");
	}
}