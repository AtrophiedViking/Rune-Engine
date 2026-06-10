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
#include "Assets/AssetManager.h"

namespace Rune {

	class Renderer;
	struct ApplicationConfig {
		std::string Name;
		WindowConfig windowConfig;
	};
	class Application
	{
	public: //Application Commands
		Application(const ApplicationConfig& config = ApplicationConfig());
		virtual ~Application();
		
		virtual std::shared_ptr<Window> WindowCreate(const WindowConfig& config = WindowConfig());

		virtual void Run();
		virtual void Stop();

		virtual void WindowRequestClose(Window* window);

		virtual void RaiseEvent(Event& event);

	public: //Global Ownership
		Logger& LoggerGet() { return *m_Logger; }
		ApplicationConfig* ConfigGet() { return &m_Config; }
		Renderer& RendererGet() const { return *Get().m_Renderer; }
		AssetManager& AssetManagerGet() const { return *Get().m_AssetManager; };
		std::shared_ptr<Window> WindowGet() const { return m_Windows.empty() ? nullptr : m_Windows.front(); }

		static Application& Get();
		static float TimeGet();

	private: //Private Members 
		std::unique_ptr<Rune::Logger> m_Logger;
		ApplicationConfig m_Config;
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<AssetManager> m_AssetManager;

		std::vector<std::shared_ptr<Window>> m_Windows;
		std::vector<Window*> m_WindowCloseQueue;
		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		bool m_Running = false;
		friend class Layer;
	};
}