#include "Application-Client.h"
#include <GLFW/glfw3.h>
#include "log/log.h"
#include <iostream>
#include <glm/glm.hpp>
#include <assert.h>
#include <ranges>

namespace Rune
{
	static Application* s_Application = nullptr;
	static int s_GLFWRefCount = 0;


	static void GLFWErrorCallback(int error, const char* description)
	{
		RUNE_ERROR("[GLFW Error Code: {}]: {}", error, description);
	}

	Application::Application(const ApplicationConfig& config)
		: s_Config(config)
	{
		s_Application = this;

		s_Logger = std::make_unique<Rune::Logger>("Rune");

		if (s_GLFWRefCount++ == 0)
		{
			glfwSetErrorCallback(GLFWErrorCallback);
			RUNE_ASSERT(glfwInit(),"Failed to initialize GLFW\n");
			
			RUNE_DEBUG("GLFW initialized!");
		}

		if (s_Config.windowConfig.Title.empty())
			s_Config.windowConfig.Title = s_Config.Name;

		s_Config.windowConfig.EventCallback = [this](Event& event) { RaiseEvent(event); };
		
		s_Renderer = std::make_unique<Renderer>();

		// Log some basic info about the app and system
		VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
		vkGetPhysicalDeviceProperties2(s_Renderer->PhysicalDeviceGet(), &deviceProperties);

		RUNE_INFO("{} Initialized!\n\t\t      GLFW {}\n\t\t      Vulkan {}.{}\n\t\t      {}",
			s_Config.Name,
			glfwGetVersionString(),
			VK_VERSION_MAJOR(VK_API_VERSION_1_3),
			VK_VERSION_MINOR(VK_API_VERSION_1_3),
			deviceProperties.properties.deviceName);
	}
	Application::~Application()
	{
		
		// Make sure the window is destroyed while GLFW is still initialized
		for (auto& wptr : m_Windows)
		{
			if (wptr)
				wptr->Destroy();
		}
		m_Windows.clear();

		if (s_Renderer)
			s_Renderer.reset();

		if (--s_GLFWRefCount == 0)
		{
			glfwTerminate();
			RUNE_DEBUG("GLFW Terminated!");
		}

		RUNE_INFO("{} Terminated!", s_Config.Name);

		if (s_Logger)
			s_Logger.reset();

		s_Application = nullptr;

	}

	std::shared_ptr<Window> Application::WindowCreate(const WindowConfig& config)
	{
		WindowConfig cfg = config;
		if (cfg.Title.empty())
			cfg.Title = s_Config.Name;
		cfg.EventCallback = [this](Event& event) { RaiseEvent(event); };
		auto window = std::make_shared<Window>(cfg);
		window->Create();
		m_Windows.push_back(window);
		return window;
	}

	void Application::Run()
	{
		m_Running = true;

		float lastTime = GetTime();

		// Main Application loop
		while (m_Running)
		{
			glfwPollEvents();

			// Process deferred window destruction here (safe point, outside callbacks)
			for (Rune::Window* w : m_WindowCloseQueue)
			{
				if (!w) continue;

				// find the shared_ptr owning this Window*
				auto it = std::find_if(m_Windows.begin(), m_Windows.end(),
					[w](const std::shared_ptr<Window>& sp) { return sp.get() == w; });

				if (it != m_Windows.end())
				{
					// destroy the GLFW handle and remove from our list
					(*it)->Destroy();
					m_Windows.erase(it);
				}
			}
			m_WindowCloseQueue.clear();

			// If no windows remain, stop the app
			if (m_Windows.empty())
			{
				Stop();
				break;
			}

			// replace the section in Application::Run that updates/render the global m_LayerStack
			// with per-window calls. Show only the replacement block for context.

			float currentTime = GetTime();
			float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
			lastTime = currentTime;

			// Per-window update + render: run each window's layer stack and then update the window
			for (auto& win : m_Windows)
			{
				if (!win || !win->IsValid())
					continue;

				// Layers attached to this window
				win->OnUpdate(timestep);
				win->OnRender();

				// Platform-specific per-window update (swap/poll/etc.)
				win->Update();
			}
			// update primary window if present
			if (!m_Windows.empty())
				m_Windows.front()->Update();
		}
	}
	void Application::Stop()
	{
		m_Running = false;
	}
	
	void Application::RaiseEvent(Event& event)
	{
	}
	void Application::WindowRequestClose(Window* window)
	{
		if (!window)
			return;
		if (std::find(m_WindowCloseQueue.begin(), m_WindowCloseQueue.end(), window) == m_WindowCloseQueue.end())
			m_WindowCloseQueue.push_back(window);
	}

	Application& Application::Get()
	{
		assert(s_Application);
		return *s_Application;
	}
	float Application::GetTime()
	{
		return (float)glfwGetTime();
	}
}