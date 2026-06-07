#pragma once
#include <vulkan/vulkan.h>
#include <memory>

namespace Rune
{
	class Window;
	class SwapchainResources;
	class Swapchain
	{
	public:
		Swapchain(Window* window);
		~Swapchain();
		void Create(Window* window);
		void Destroy();
		void Recreate();
		void Present();

	public:
		VkSwapchainKHR HandleGet() const { return m_Handle; }
		Window* OwnerGet() const { return (Window*)m_Owner; }

	private:
		VkExtent2D m_Extent;
		Window* m_Owner = nullptr;
		VkSwapchainKHR m_Handle = VK_NULL_HANDLE;
		std::unique_ptr<SwapchainResources> m_Resources = nullptr;
	};
}