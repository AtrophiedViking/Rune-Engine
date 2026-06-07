#pragma once
#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>

#include "log/log.h"
#include "Layer.h"
#include "Window.h"
#include "Event.h"
#include "GUI/Gui.h"

namespace Rune {

	class Renderer;
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

	public:
		std::shared_ptr<Window> WindowGet() const { return m_Windows.empty() ? nullptr : m_Windows.front(); }
		
		static Renderer& RendererGet() { return *Get().m_Renderer; }

		static Application& Get();
		static float TimeGet();

	public:
		std::unique_ptr<Rune::Logger> s_Logger;
		ApplicationConfig s_Config;

	private:
		std::unique_ptr<Renderer> m_Renderer;
		std::vector<std::shared_ptr<Window>> m_Windows;
		std::vector<Window*> m_WindowCloseQueue;
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		bool m_Running = false;
		friend class Layer;
	};
}