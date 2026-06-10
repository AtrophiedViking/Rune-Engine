#pragma once
#include "AssetTypes.h"
#include "fastgltf/core.hpp"
namespace Rune
{
	class AssetManager
	{
	public: 
		AssetId GLBLoad(const std::filesystem::path& file);
		AssetId TextureLoad(const std::filesystem::path& file);

		const MeshAsset& MeshGet(AssetId h) const;
		const TextureAsset& TextureGet(AssetId h) const;

		//TODO: static AssetManager& Get();

	private:
		std::vector<MeshAsset> m_Meshes;
		std::vector<TextureAsset> m_Textures;
		std::vector<MaterialAsset> m_Materials;
	};
}