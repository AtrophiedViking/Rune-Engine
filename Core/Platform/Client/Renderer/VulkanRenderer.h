#pragma once
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
		
	private:
		void InstanceCreate(const char* name);
		void InstanceDestroy();
		void PhysicalDevicePick();
		void LogicalDeviceCreate();
		void LogicalDeviceDestroy();

	private:
		VkInstance m_Instance = nullptr;
		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkDevice m_LogicalDevice = nullptr;
	};
}

