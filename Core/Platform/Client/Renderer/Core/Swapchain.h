#pragma once
#include "../../Application-Client.h"

namespace Rune
{
	class Swapchain
	{
	public:
		Swapchain(Window* window);
		~Swapchain();
		void Create(Window* window);
		void Destroy();
		void Recreate();
		void Present();
	private:
		Window* GetOwner() const { return (Window*)m_Owner; }

	private:
		VkExtent2D m_Extent;
		Window* m_Owner = nullptr;
		VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
	};
}