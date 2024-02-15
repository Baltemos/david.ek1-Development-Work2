#include "SpriteManager.h"
#include <tge/settings/settings.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <tge/texture/TextureManager.h>
#include <tge/util/StringCast.h>

Tga::SpriteSharedData SpriteManager::Get(const std::string& anID)
{
	auto entry = mySpriteDataMap.find(anID);
	if (entry == mySpriteDataMap.end())
	{
		return mySpriteDataMap["error"];
		//throw std::exception(("Could not find SpriteSharedData associated with ID '" + anID + "'.").c_str());
	}
	return entry->second;
}

void SpriteManager::Load(const std::string& anAssetPath)
{
	nlohmann::json sprites;
	std::ifstream file(Tga::Settings::ResolveGameAssetPath(anAssetPath));
	file >> sprites;
	file.close();

	for (auto& sprite : sprites)
	{
		auto engine = Tga::Engine::GetInstance();

		Tga::SpriteSharedData shared;
		shared.myTexture = engine->GetTextureManager().GetTexture(string_cast<std::wstring>(sprite["Texture"].get<std::string>()).c_str());

		if (sprite.contains("Shader") && !sprite["Shader"].is_null())
		{
			throw std::exception(("Could not load shader for sprite '" + anAssetPath + "'. Shaders are not implemented.").c_str());
		}

		auto& normalMap = sprite["NormalMap"];
		if (!normalMap.is_null())
		{
			shared.myMaps[Tga::ShaderMap::NORMAL_MAP] = engine->GetTextureManager().GetTexture(string_cast<std::wstring>(sprite.get<std::string>()).c_str());
		}

		mySpriteDataMap.emplace(sprite["ID"], shared);
	}
}
