#include "Swapchain.h"
#include "Resources/SwapchainResources.h"
#include "../../Application-Client.h"
#include "../VulkanRenderer.h"

namespace Rune
{
	Swapchain::Swapchain(Window* window)
	{
		Create(window);
		RUNE_INFO("Created Swapchain for Window: {}", window->TitleGet());
	}
	Swapchain::~Swapchain()
	{
		Destroy();
	}

	void Swapchain::Create(Window* window)
	{
		m_Owner = window;
		
		VkSurfaceCapabilitiesKHR surfaceCaps{};
		RUNE_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Application::Get().RendererGet().PhysicalDeviceGet(), window->SurfaceGet(), &surfaceCaps) == VK_SUCCESS, "Failed to get surface capabilities!");

		VkExtent2D swapchainExtent{ surfaceCaps.currentExtent };
		if (surfaceCaps.currentExtent.width == 0xFFFFFFFF) {
			swapchainExtent = { .width = static_cast<uint32_t>(window->GetFramebufferSize().x), .height = static_cast<uint32_t>(window->GetFramebufferSize().y) };
		}

		const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_SRGB };
		VkSwapchainCreateInfoKHR swapchainCreateInfo{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = window->SurfaceGet(),
			.minImageCount = surfaceCaps.minImageCount,
			.imageFormat = imageFormat,
			.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
			.imageExtent{.width = swapchainExtent.width, .height = swapchainExtent.height },
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = VK_PRESENT_MODE_FIFO_KHR
		};
		RUNE_ASSERT(vkCreateSwapchainKHR(Application::Get().RendererGet().LogicalDeviceGet(), &swapchainCreateInfo, nullptr, &m_Handle) == VK_SUCCESS, "Failed to create swapchain!");

		RUNE_DEBUG("Created New Swapchain!");

		m_Resources = std::make_unique<SwapchainResources>(*this);
	}
	void Swapchain::Destroy()
	{
		m_Resources = nullptr;
		vkDestroySwapchainKHR(Application::Get().RendererGet().LogicalDeviceGet(), m_Handle, nullptr);
		RUNE_DEBUG("Destroyed Swapchain!");
	}
	void Swapchain::Recreate()
	{
	}
	void Swapchain::Present()
	{
	}
}