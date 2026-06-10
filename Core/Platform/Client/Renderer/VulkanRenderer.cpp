#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#include "VulkanRenderer.h"
#include "../Platform/Client/Application-Client.h"
#include "log/log.h"

namespace Rune
{
	Renderer::Renderer()
	{
		InstanceCreate(Application::Get().ConfigGet()->Name.c_str());
		PhysicalDevicePick();
		LogicalDeviceCreate();
		AllocatorCreate();
		RUNE_DEBUG("Renderer Initialized!");
	}
	Renderer::~Renderer()
	{
		AllocatorDestroy();
		LogicalDeviceDestroy();
		InstanceDestroy();
		RUNE_DEBUG("Renderer Terminated!");
	}

	void Renderer::InstanceCreate(const char* name)
	{
		uint32_t m_RequiredExtensionsCount;
		const char** m_RequiredExtensions = glfwGetRequiredInstanceExtensions(&m_RequiredExtensionsCount);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = name;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Rune Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = m_RequiredExtensionsCount;
		createInfo.ppEnabledExtensionNames = m_RequiredExtensions;
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;

		RUNE_ASSERT(vkCreateInstance(&createInfo, nullptr, &m_Instance) == VK_SUCCESS,"Failed to create Vulkan instance!");
	}
	void Renderer::InstanceDestroy()
	{
		if (m_Instance)
			vkDestroyInstance(m_Instance, nullptr);
	}

	void Renderer::PhysicalDevicePick()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);
		RUNE_ASSERT(deviceCount > 0, "Failed to find GPUs with Vulkan support!");
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
		for (const auto& device : devices)
		{
			m_PhysicalDevice = device;
			break; // For now, just pick the first one. In a real app, you'd want to check for features and suitability.
		}
		RUNE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!");
	}

	void Renderer::LogicalDeviceCreate()
	{
		// Queue family
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		RUNE_ASSERT(queueFamilyCount > 0, "Failed to find any queue families!");
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());
		uint32_t m_queueFamilyIndex;
		for (uint32_t i = 0; i < queueFamilies.size(); i++)
		{
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				m_queueFamilyIndex = i;
				break;
			}
		}
		RUNE_ASSERT(glfwGetPhysicalDevicePresentationSupport(m_Instance, m_PhysicalDevice, m_queueFamilyIndex), "Failed to find a queue family that supports presentation!");

		const float qfpriorities{ 1.0f };
		VkDeviceQueueCreateInfo queueCreateInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = m_queueFamilyIndex,
			.queueCount = 1,
			.pQueuePriorities = &qfpriorities
		};

		// Device extensions
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		// Device features
		VkPhysicalDeviceVulkan12Features enabledVk12Features{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
			.descriptorIndexing = true,
			.shaderSampledImageArrayNonUniformIndexing = true,
			.descriptorBindingVariableDescriptorCount = true,
			.runtimeDescriptorArray = true,
			.bufferDeviceAddress = true
		};
		VkPhysicalDeviceVulkan13Features enabledVk13Features{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
			.pNext = &enabledVk12Features,
			.synchronization2 = true,
			.dynamicRendering = true,
		};
		VkPhysicalDeviceFeatures enabledVk10Features{
			.samplerAnisotropy = VK_TRUE
		};
		VkDeviceCreateInfo deviceCreateInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &enabledVk13Features,
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queueCreateInfo,
			.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
			.ppEnabledExtensionNames = deviceExtensions.data(),
			.pEnabledFeatures = &enabledVk10Features
		};

		RUNE_ASSERT(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_LogicalDevice) == VK_SUCCESS, "Failed to create logical device!");
	}
	void Renderer::LogicalDeviceDestroy()
	{
		if (m_LogicalDevice)
			vkDestroyDevice(m_LogicalDevice, nullptr);
	}
	
	void Renderer::AllocatorCreate()
	{
		VmaVulkanFunctions vkFunctions{
			.vkGetInstanceProcAddr = vkGetInstanceProcAddr,
			.vkGetDeviceProcAddr = vkGetDeviceProcAddr,
			.vkCreateImage = vkCreateImage
		};
		VmaAllocatorCreateInfo allocatorCreateInfo{
			.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
			.physicalDevice = m_PhysicalDevice,
			.device = m_LogicalDevice,
			.pVulkanFunctions = &vkFunctions,
			.instance = m_Instance
		};
		RUNE_ASSERT(vmaCreateAllocator(&allocatorCreateInfo, &m_allocator) == VK_SUCCESS, "Failed to create VMA allocator!");
	}
	void Renderer::AllocatorDestroy()
	{
		if (m_allocator)
			vmaDestroyAllocator(m_allocator);
	}
}