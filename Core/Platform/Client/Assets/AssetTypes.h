#pragma once
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include "glm/glm.hpp"
namespace Rune {
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	using AssetId = uint32_t;

	struct MaterialAsset
	{
		glm::vec4 baseColorFactor{ 1.0f };
		float metallicFactor{ 1.0f };
		float roughnessFactor{ 1.0f };

		// Store the loaded Texture IDs managed by our AssetManager
		AssetId baseColorTextureId{ 0 };
		AssetId metallicRoughnessTextureId{ 0 };
	};
	struct MeshPrimitive
	{
		uint32_t firstIndex{ 0 };
		uint32_t indexCount{ 0 };
		AssetId materialId{ 0 };
	};
	struct MeshAsset
	{
		VkBuffer vertexBuffer; 
		VkBuffer indexBuffer;
		VmaAllocation vertexAlloc, indexAlloc;
		uint32_t indexCount;

		std::vector<MeshPrimitive> primitives;
	};

	struct TextureAsset
	{
		VkImage image;
		VkImageView view;
		VmaAllocation alloc;
		VkFormat format;
	};
}