#include "SwapchainResources.h"
#include "../../../Application-Client.h"
#include "../../VulkanRenderer.h"
#include "../Swapchain.h"
namespace Rune
{
	SwapchainResources::SwapchainResources(Swapchain& swapchain)
	{
		ImagesCreate(swapchain.HandleGet());
		ImageViewsCreate(swapchain.HandleGet());

		DepthResourcesCreate(swapchain.OwnerGet());
	}
	SwapchainResources::~SwapchainResources()
	{
		ImageViewsDestroy();
	}
	void SwapchainResources::ImagesCreate(VkSwapchainKHR swapchain)
	{
		uint32_t imageCount{ 0 };
		RUNE_ASSERT(vkGetSwapchainImagesKHR(Application::Get().RendererGet().LogicalDeviceGet(), swapchain, &imageCount, nullptr) == VK_SUCCESS, "Failed to get swapchain image count!");
		m_Images.resize(imageCount);
		RUNE_ASSERT(vkGetSwapchainImagesKHR(Application::Get().RendererGet().LogicalDeviceGet(), swapchain, &imageCount, m_Images.data()) == VK_SUCCESS, "Failed to get swapchain images!");
		m_ImageViews.resize(imageCount);

		RUNE_DEBUG("Acquired {} swapchain images!", imageCount);
	}

	void SwapchainResources::ImageViewsCreate(VkSwapchainKHR swapchain)
	{
		for (size_t i = 0; i < m_Images.size(); i++)
		{
			VkImageViewCreateInfo createInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = m_Images[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_B8G8R8A8_SRGB,
				.components{.r = VK_COMPONENT_SWIZZLE_IDENTITY, .g = VK_COMPONENT_SWIZZLE_IDENTITY, .b = VK_COMPONENT_SWIZZLE_IDENTITY, .a = VK_COMPONENT_SWIZZLE_IDENTITY },
				.subresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
			};
			RUNE_ASSERT(vkCreateImageView(Application::Get().RendererGet().LogicalDeviceGet(), &createInfo, nullptr, &m_ImageViews[i]) == VK_SUCCESS, "Failed to create image view for swapchain image!");
		}
		RUNE_DEBUG("Created {} swapchain image views!", m_ImageViews.size());
	}
	void SwapchainResources::ImageViewsDestroy()
	{
		for (VkImageView imageView : m_ImageViews)
			vkDestroyImageView(Application::Get().RendererGet().LogicalDeviceGet(), imageView, nullptr);
		RUNE_DEBUG("Destroyed {} swapchain image views!", m_ImageViews.size());
	}

	void SwapchainResources::DepthResourcesCreate(Window* window)
	{
		std::vector<VkFormat> depthFormatList = {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
		VkFormat depthFormat{ VK_FORMAT_UNDEFINED };
		for (VkFormat& format : depthFormatList) {
			VkFormatProperties2 formatProperties{ .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2 };
			vkGetPhysicalDeviceFormatProperties2(Application::Get().RendererGet().PhysicalDeviceGet(), format, &formatProperties);
			if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
				depthFormat = format;
				break;
			}
		}

		VkImageCreateInfo depthImageCI
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = depthFormat,
			.extent{.width = static_cast<uint32_t>(window->GetFramebufferSize().x), .height = static_cast<uint32_t>(window->GetFramebufferSize().y), .depth = 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};
	}
	void SwapchainResources::DepthResourcesDestroy()
	{
	}

	void SwapchainResources::Recreate(VkSwapchainKHR swapchain)
	{
	}
}