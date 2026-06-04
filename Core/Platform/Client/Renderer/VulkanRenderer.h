#pragma once
#include <vk_mem_alloc.h>
#include "../Platform/Client/Window.h"

namespace Rune
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		

		VkInstance InstanceGet() const { return m_Instance; };
		VkPhysicalDevice PhysicalDeviceGet() const { return m_PhysicalDevice; };
		VkDevice LogicalDeviceGet() const { return m_LogicalDevice; };
		VmaAllocator AllocatorGet() const { return m_allocator; };

	private:
		void InstanceCreate(const char* name);
		void InstanceDestroy();
		void PhysicalDevicePick();
		void LogicalDeviceCreate();
		void LogicalDeviceDestroy();
		void AllocatorCreate();
		void AllocatorDestroy();

	private:
		VmaAllocator m_allocator = nullptr;
		VkInstance m_Instance = nullptr;
		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkDevice m_LogicalDevice = nullptr;
	};
}

