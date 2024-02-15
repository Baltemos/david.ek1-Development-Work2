#pragma once
#include <unordered_map>
#include <tge/sprite/sprite.h>

class SpriteManager
{
public:
	SpriteManager() = default;
	~SpriteManager() = default;

	//Retrieves the shared pointer to the associated instance of SpriteSharedData.
	Tga::SpriteSharedData Get(const std::string& anID);
	void Load(const std::string& anAssetPath);

private:
	std::unordered_map<std::string, Tga::SpriteSharedData> mySpriteDataMap;
};