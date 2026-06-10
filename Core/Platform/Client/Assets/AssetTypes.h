#pragma once
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include "glm/glm.hpp"
namespace Rune {

	// ─── Handle ───────────────────────────────────────────────────────────────
	static constexpr uint32_t INVALID_ASSET_ID = UINT32_MAX;
	using AssetId = uint32_t;

	struct AssetHandle
	{
		AssetId index = INVALID_ASSET_ID;
		bool IsValid() const { return index != INVALID_ASSET_ID; }
		bool operator==(const AssetHandle&) const = default;
	};

	// ─── Geometry ─────────────────────────────────────────────────────────────
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 tangent;   // w = bitangent sign
		glm::vec2 texCoord;
	};

	// ─── Sub-primitive (one draw call worth of indices + material) ─────────────
	struct MeshPrimitive
	{
		uint32_t  firstIndex{ 0 };
		uint32_t  indexCount{ 0 };
		AssetId   materialId{ INVALID_ASSET_ID };
	};

	// ─── GPU mesh (owns VkBuffers) ─────────────────────────────────────────────
	struct MeshAsset
	{
		VkBuffer      vertexBuffer = VK_NULL_HANDLE;
		VkBuffer      indexBuffer = VK_NULL_HANDLE;
		VmaAllocation vertexAlloc = VK_NULL_HANDLE;
		VmaAllocation indexAlloc = VK_NULL_HANDLE;
		uint32_t      indexCount = 0;

		std::vector<MeshPrimitive> primitives;
	};

	// ─── GPU texture ──────────────────────────────────────────────────────────
	struct TextureAsset
	{
		VkImage       image = VK_NULL_HANDLE;
		VkImageView   view = VK_NULL_HANDLE;
		VmaAllocation alloc = VK_NULL_HANDLE;
		VkFormat      format = VK_FORMAT_UNDEFINED;
	};

	// ─── PBR material (holds handles into TextureAsset pool) ──────────────────
	struct MaterialAsset
	{
		glm::vec4 baseColorFactor{ 1.0f };
		float     metallicFactor{ 1.0f };
		float     roughnessFactor{ 1.0f };

		AssetId   baseColorTextureId{ INVALID_ASSET_ID };
		AssetId   metallicRoughnessTextureId{ INVALID_ASSET_ID };
		AssetId   normalTextureId{ INVALID_ASSET_ID };
	};
}