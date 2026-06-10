#include "AssetManager.h"
#include "log/log.h"
#include <fastgltf/glm_element_traits.hpp> 
#include <fastgltf/tools.hpp>

namespace Rune
{
	AssetId AssetManager::GLBLoad(const std::filesystem::path& file)
	{
        const std::filesystem::path baseDirRel = "../Client/Assets/Models";
        const std::filesystem::path baseDir = std::filesystem::weakly_canonical(std::filesystem::absolute(baseDirRel));
        const std::filesystem::path fullPath = (baseDir / file).lexically_normal();

		fastgltf::Parser parser;

		auto Data = fastgltf::GltfDataBuffer::FromPath(fullPath);
        if (Data.error() != fastgltf::Error::None) {
            RUNE_WARN("GLBLoad: failed to load file '{}': {} - {}", fullPath.string(),
                static_cast<uint64_t>(Data.error()), fastgltf::getErrorMessage(Data.error()));
            return INVALID_ASSET_ID; // or appropriate failure handling
        }

		auto Options = fastgltf::Options::DontRequireValidAssetMember;
		auto Asset = parser.loadGltfBinary(Data.get(), fullPath.parent_path(), Options);
        if (Asset.error() != fastgltf::Error::None) {
            RUNE_WARN("GLBLoad: failed to parse '{}': ERROR[{}] - {}", fullPath.string(),
                static_cast<uint64_t>(Asset.error()), fastgltf::getErrorMessage(Asset.error()));
            return INVALID_ASSET_ID;
        }

       // ---------------------------------------------------------
       // STEP 1: LOAD ALL MODEL IMAGES / TEXTURES FIRST
       // ---------------------------------------------------------
        std::vector<AssetId> localToGlobalTextureMap(Asset->textures.size(), 0);

        for (size_t i = 0; i < Asset->textures.size(); ++i)
        {
            const auto& gltfTexture = Asset->textures[i];

            if (!gltfTexture.imageIndex.has_value()) {
                continue;
            }

            size_t imgIdx = gltfTexture.imageIndex.value();
            const auto& gltfImage = Asset->images[imgIdx];

            // Check if the image source is stored as an external URI path on disk
            if (std::holds_alternative<fastgltf::sources::URI>(gltfImage.data))
            {
                const auto& filePath = std::get<fastgltf::sources::URI>(gltfImage.data);

                if (filePath.uri.isLocalPath())
                {
                    // Build the absolute file path on disk relative to the glTF container location
                    std::filesystem::path absolutePath = file.parent_path() / filePath.uri.fspath();
                    localToGlobalTextureMap[i] = this->TextureLoad(absolutePath);
                }
            }
            // Check if the image source is embedded as an raw array vector inside the JSON
            else if (std::holds_alternative<fastgltf::sources::Vector>(gltfImage.data))
            {
                const auto& rawData = std::get<fastgltf::sources::Vector>(gltfImage.data);
                const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(rawData.bytes.data());
                size_t dataSize = rawData.bytes.size();

                RUNE_DEBUG("[AssetManager] Found raw vector image memory data. Size: {}", dataSize);
                // TODO: localToGlobalTextureMap[i] = this->loadTextureFromMemory(dataPtr, dataSize);
            }
            // Check if the image source is packed inside an internal GLB binary buffer chunk
            else if (std::holds_alternative<fastgltf::sources::BufferView>(gltfImage.data))
            {
                const auto& bufferViewRef = std::get<fastgltf::sources::BufferView>(gltfImage.data);
                const auto& view = Asset->bufferViews[bufferViewRef.bufferViewIndex];
                const auto& buffer = Asset->buffers[view.bufferIndex];

                // Ensure the underlying buffer storage container itself is a flat vector array
                if (std::holds_alternative<fastgltf::sources::Vector>(buffer.data))
                {
                    const auto& bytes = std::get<fastgltf::sources::Vector>(buffer.data);
                    const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>( bytes.bytes.data() + view.byteOffset);
                    size_t dataSize = view.byteLength;

                    RUNE_DEBUG("[AssetManager] Found binary GLB BufferView image. Size: ", dataSize);
                    // TODO: localToGlobalTextureMap[i] = this->loadTextureFromMemory(dataPtr, dataSize);
                }
            }
        }

        // ---------------------------------------------------------
        // STEP 2: LOAD ALL MATERIALS AND MAP CAPTURED TEXTURES
        // ---------------------------------------------------------
        std::vector<AssetId> localToGlobalMaterialMap(Asset->materials.size(), 0);

        for (size_t i = 0; i < Asset->materials.size(); ++i)
        {
            const auto& gltfMaterial = Asset->materials[i];
            MaterialAsset newMaterial{};

            // Extract foundational PBR multipliers
            newMaterial.baseColorFactor = glm::vec4(
                gltfMaterial.pbrData.baseColorFactor[0],
                gltfMaterial.pbrData.baseColorFactor[1],
                gltfMaterial.pbrData.baseColorFactor[2],
                gltfMaterial.pbrData.baseColorFactor[3]
            );
            newMaterial.metallicFactor = gltfMaterial.pbrData.metallicFactor;
            newMaterial.roughnessFactor = gltfMaterial.pbrData.roughnessFactor;

            // Extract the texture indices if assigned by the designer
            if (gltfMaterial.pbrData.baseColorTexture.has_value())
            {
                size_t texIdx = gltfMaterial.pbrData.baseColorTexture->textureIndex;
                newMaterial.baseColorTextureId = localToGlobalTextureMap[texIdx];
            }
            if (gltfMaterial.pbrData.metallicRoughnessTexture.has_value())
            {
                size_t texIdx = gltfMaterial.pbrData.metallicRoughnessTexture->textureIndex;
                newMaterial.metallicRoughnessTextureId = localToGlobalTextureMap[texIdx];
            }

            // Push material to manager
            m_Materials.push_back(newMaterial);
            localToGlobalMaterialMap[i] = m_Materials.size() - 1;
            RUNE_DEBUG("Created Material {}", m_Materials.size() - 1)
        }

        // ---------------------------------------------------------
        // STEP 3: EXTRACT GEOMETRY AND PAIR PRIMITIVES TO MATERIALS
        // ---------------------------------------------------------
        MeshAsset newMesh{};
        std::vector<Vertex> globalVertices;
        std::vector<uint32_t> globalIndices;

        for (const auto& gltfMesh : Asset->meshes)
        {
            for (const auto& primitive : gltfMesh.primitives)
            {
                MeshPrimitive subPrimitive{};
                subPrimitive.firstIndex = static_cast<uint32_t>(globalIndices.size());

                uint32_t vertexStartOffset = static_cast<uint32_t>(globalVertices.size());

                // Assign the material lookup index directly to this specific sub-primitive primitive
                if (primitive.materialIndex.has_value())
                {
                    size_t localMatIdx = primitive.materialIndex.value();
                    subPrimitive.materialId = localToGlobalMaterialMap[localMatIdx];
                }
                else
                {
                    subPrimitive.materialId = 0; // Fallback default material asset handle
                }

                // ... [Execute POSITIONS, NORMALS, and TEXCOORDS accessor loops exactly like before] ...

                if (primitive.indicesAccessor.has_value())
                {
                    const auto& indicesAccessor = Asset->accessors[primitive.indicesAccessor.value()];
                    subPrimitive.indexCount = static_cast<uint32_t>(indicesAccessor.count);

                    size_t startIndexCount = globalIndices.size();
                    globalIndices.resize(startIndexCount + indicesAccessor.count);

                    fastgltf::iterateAccessorWithIndex<uint32_t>(
                        Asset.get(), indicesAccessor,
                        [&](uint32_t gltfIndex, size_t index) {
                            globalIndices[startIndexCount + index] = gltfIndex + vertexStartOffset;
                        }
                    );
                }

                newMesh.primitives.push_back(subPrimitive);
            }
        }

        // 5. STAGE & UPLOAD DATA TO THE GPU
        // Pass your populated staging vectors directly to your engine's Vulkan hardware wrappers
        newMesh.indexCount = static_cast<uint32_t>(globalIndices.size());

        // --- Pseudo-code context showing Vulkan initialization hookups ---
        // newMesh.vertexBuffer = CreateVulkanBuffer(globalVertices.data(), globalVertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        // newMesh.indexBuffer  = CreateVulkanBuffer(globalIndices.data(), globalIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        m_Meshes.push_back(std::move(newMesh));
        RUNE_DEBUG("Created Mesh {}", m_Meshes.size() - 1)
        return static_cast<AssetId>(m_Meshes.size() - 1);
	}


    AssetId AssetManager::TextureLoad(const std::filesystem::path& file)
    {
        TextureAsset newTexture;
        
        
        m_Textures.push_back(newTexture);
        return static_cast<AssetId>(m_Textures.size() - 1);
    }

    const MeshAsset& AssetManager::MeshGet(AssetId h) const
    {
        return m_Meshes[h];
    }

    const TextureAsset& AssetManager::TextureGet(AssetId h) const
    {
        return m_Textures[h];
    }
    //TODO
    //AssetManager& AssetManager::Get()
    //{
    //    return ;
    //}
}