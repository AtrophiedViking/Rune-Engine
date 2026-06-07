#pragma once
#include <vulkan/vulkan.h>
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

		void ImagesCreate(VkSwapchainKHR swapchain);

		void ImageViewsCreate(VkSwapchainKHR swapchain);
		void ImageViewsDestroy();

		void DepthResourcesCreate(Window* window);
		void DepthResourcesDestroy();

		void Recreate(VkSwapchainKHR swapchain);

	private:
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
	};
}