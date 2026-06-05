#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "Layer.h"
#include "Event.h"

namespace Rune
{
	struct WindowConfig
	{
		std::string Title;
		uint32_t Width = 1280;
		uint32_t Height = 720;

		bool Fullscreen = false;
		bool Resizable = true;
		bool VSync = true;

		using EventCallbackFn = std::function<void(Event&)>;
		EventCallbackFn EventCallback;
	};

	class Swapchain;
	class Window
	{
	public: // Window Management
		Window(const WindowConfig& config = WindowConfig());
		~Window();

		void Create();
		void Update();
		void Destroy();

		void SurfaceCreate(GLFWwindow* m_Handle);
		void SurfaceDestroy(GLFWwindow* window);
		VkSurfaceKHR SurfaceGet() const { return m_Surface; };

		void OnUpdate(float timestep);
		void OnRender();
		bool ShouldClose() const;

		void RaiseEvent(Event& event);

	public: //	Event Handling
		glm::vec2 GetFramebufferSize() const;
		glm::vec2 GetMousePos() const;
		GLFWwindow* GetHandle() const { return m_Handle; }
		static Window& Get();
		bool IsValid() const { return m_Handle != nullptr; }
		

	public: //Layer Management
		template<typename TLayer>
			requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer()
		{
			// Create layer, set owner, then push so owner is always valid
			auto layer = std::make_unique<TLayer>();
			layer->SetOwner(this);
			m_LayerStack.push_back(std::move(layer));
		}

		template<typename TLayer, typename... Args>
			requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer(Args&&... args)
		{
			auto layer = std::make_unique<TLayer>(std::forward<Args>(args)...);
			layer->SetOwner(this);
			m_LayerStack.push_back(std::move(layer));
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


	private: //Member Variables
		WindowConfig m_Config;
		GLFWwindow* m_Handle = nullptr;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		std::unique_ptr<Swapchain> m_Swapchain = nullptr;
		std::vector<std::unique_ptr<Layer>> m_LayerStack;

		friend class Layer;
	};
}