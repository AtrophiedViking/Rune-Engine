#pragma once
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <vector>

namespace Rune
{
	class Window;
	class Swapchain;
	class SwapchainResources
	{
	public:
		SwapchainResources(Swapchain& swapchain);
		~SwapchainResources();

		void ImagesAquire(VkSwapchainKHR swapchain);

		void ImageViewsCreate(VkSwapchainKHR swapchain);
		void ImageViewsDestroy();

		void DepthImageCreate(Window* window);
		void DepthImageDestroy();

		void DepthImageViewCreate();
		void DepthImageViewDestroy();

		void Recreate(VkSwapchainKHR swapchain);

	private:
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		VkFormat m_DepthFormat = { VK_FORMAT_UNDEFINED };
		VkImage m_DepthImage = VK_NULL_HANDLE;
		VmaAllocation m_DepthImageAllocation = VK_NULL_HANDLE;
		VkImageView m_DepthImageView = VK_NULL_HANDLE;
	};
}