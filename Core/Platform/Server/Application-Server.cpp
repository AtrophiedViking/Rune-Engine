#include "Application-Server.h"

namespace Rune
{
	static Application* s_Application = nullptr;

	Application::Application(const ApplicationConfig& config)
		: m_Config(config)
	{
		s_Application = this;
	}
	Application::~Application()
	{

	}

	template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
	void PushLayer()
	{
		m_LayerStack.push_back(std::make_unique<TLayer>());
	}

	template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
	TLayer* GetLayer()
	{
		for (const auto& layer : m_LayerStack)
		{
			if (auto casted = dynamic_cast<TLayer*>(layer.get()))
				return casted;
		}
		return nullptr;
	}

	Application& Application::Get()
	{
		return *s_Application;
	}
}