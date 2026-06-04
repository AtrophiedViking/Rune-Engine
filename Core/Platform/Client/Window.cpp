#include <ranges>
#include "../Platform/Client/Application-Client.h"
#include "window.h"
#include "WindowEvents.h"
#include "InputEvents.h"
#include "log/log.h"
namespace Rune
{
	Window::Window(const WindowConfig& config)
		: m_Config(config)
	{
	}
	Window::~Window()
	{
		Destroy();
	}
	void Window::Create()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_MAXIMIZED, m_Config.Fullscreen);
		glfwWindowHint(GLFW_RESIZABLE, m_Config.Resizable && !m_Config.Fullscreen );

		m_Handle = glfwCreateWindow(m_Config.Width, m_Config.Height,
			m_Config.Title.c_str(), nullptr, nullptr);

		RUNE_ASSERT(m_Handle, "Failed to create GLFW window!");


		glfwSetWindowUserPointer(m_Handle, this);

		if (!glfwVulkanSupported())
		{
			RUNE_ASSERT(glfwVulkanSupported(), "Vulkan not supported!");
		}
		else
		{
			
			glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* handle)
				{
					Window& window = *((Window*)glfwGetWindowUserPointer(handle));

					WindowClosedEvent event(&window);
					window.RaiseEvent(event);
				});

			glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* handle, int width, int height)
				{
					Window& window = *((Window*)glfwGetWindowUserPointer(handle));

					WindowResizeEvent event(&window, (uint32_t)width, (uint32_t)height);
					window.RaiseEvent(event);
				});

			glfwSetKeyCallback(m_Handle, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
				{
					Window& window = *((Window*)glfwGetWindowUserPointer(handle));

					switch (action)
					{
					case GLFW_PRESS:
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, action == GLFW_REPEAT);
						window.RaiseEvent(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						window.RaiseEvent(event);
						break;
					}
					}
				});

			glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* handle, int button, int action, int mods)
				{
					Window& window = *((Window*)glfwGetWindowUserPointer(handle));

					switch (action)
					{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						window.RaiseEvent(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						window.RaiseEvent(event);
						break;
					}
					}
				});

			glfwSetScrollCallback(m_Handle, [](GLFWwindow* handle, double xOffset, double yOffset)
				{
					Window& window = *((Window*)glfwGetWindowUserPointer(handle));

					MouseScrolledEvent event(xOffset, yOffset);
					window.RaiseEvent(event);
				});

			glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* handle, double x, double y)
				{
					Window& window = *((Window*)glfwGetWindowUserPointer(handle));

					MouseMovedEvent event(x, y);
					window.RaiseEvent(event);
				});
		}
		RUNE_DEBUG("Created New Window: \"{}\" ({}x{})", m_Config.Title, m_Config.Width, m_Config.Height);

		SurfaceCreate(m_Handle);
	}
	void Window::Update()
	{

	}
	void Window::Destroy()
	{
		if (!m_Handle)
			return;
		// Destroy layers first to ensure no layer calls GLFW during shutdown
		m_LayerStack.clear();
		// Then destroy the window handle
		SurfaceDestroy(m_Handle);
		glfwDestroyWindow(m_Handle);
		m_Handle = nullptr;
		RUNE_DEBUG("Destroyed Window!");

	}

	void Window::SurfaceCreate(GLFWwindow* m_Handle)
	{
		if (glfwCreateWindowSurface(Application::Get().s_Renderer->InstanceGet(), m_Handle, nullptr, &m_Surface) != VK_SUCCESS)
		{
			RUNE_ERROR("Failed to create window surface!");
			assert(false);
		}
		else 
		{
			RUNE_DEBUG("Created New SurfaceKHR!");
		}
	}
	void Window::SurfaceDestroy(GLFWwindow* window)
	{
		if (m_Surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(Application::Get().s_Renderer->InstanceGet(), m_Surface, nullptr);
			m_Surface = VK_NULL_HANDLE;
			RUNE_DEBUG("Destroyed SurfaceKHR!");
		}
	}

	void Window::RaiseEvent(Event& event)
	{
		if (m_Config.EventCallback)
			m_Config.EventCallback(event);
		for (auto& layer : std::views::reverse(m_LayerStack))
		{
			layer->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Window::OnUpdate(float timestep)
	{
		for (const auto& layer : m_LayerStack)
			layer->OnUpdate(timestep);
	}
	void Window::OnRender()
	{
		for (const auto& layer : m_LayerStack)
			layer->OnRender();
	}
	bool Window::ShouldClose() const
	{
		if (!m_Handle) return true;
		return glfwWindowShouldClose(m_Handle);
	}

	glm::vec2 Window::GetFramebufferSize() const
	{
		if (!m_Handle)
			return { 0.0f, 0.0f };
		int width, height;
		glfwGetFramebufferSize(m_Handle, &width, &height);
		return { static_cast<float>(width), static_cast<float>(height) };
	}
	glm::vec2 Window::GetMousePos() const
	{
		if (!m_Handle)
			return { 0.0f, 0.0f };
		double x, y;
		glfwGetCursorPos(m_Handle, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}
	
	Window& Window::Get()
	{
		auto ptr = Application::Get().GetWindow();
		RUNE_ASSERT(ptr, "Window::Get() called but Application has no window");
		return *ptr;
	}
}
