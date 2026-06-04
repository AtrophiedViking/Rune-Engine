#pragma once
#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>

#include "log/log.h"
#include "Renderer/VulkanRenderer.h"
#include "Layer.h"
#include "Window.h"
#include "Event.h"
#include "GUI/Gui.h"

namespace Rune {

	struct ApplicationConfig {
		std::string Name;
		WindowConfig windowConfig;
	};
	
	class Application
	{
	public:
		
		Application(const ApplicationConfig& config = ApplicationConfig());
		virtual ~Application();
		
		virtual std::shared_ptr<Window> WindowCreate(const WindowConfig& config = WindowConfig());

		virtual void Run();
		virtual void Stop();

		virtual void WindowRequestClose(Window* window);

		virtual void RaiseEvent(Event& event);

		virtual std::shared_ptr<Window> GetWindow() const { return m_Windows.empty() ? nullptr : m_Windows.front(); }
		static Application& Get();
		static float GetTime();

	public:
		std::unique_ptr<Rune::Logger> s_Logger;
		std::unique_ptr<Renderer> s_Renderer;
		ApplicationConfig s_Config;
	private:

		std::vector<std::shared_ptr<Window>> m_Windows;
		std::vector<Window*> m_WindowCloseQueue;
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		bool m_Running = false;
		friend class Layer;
	};
}