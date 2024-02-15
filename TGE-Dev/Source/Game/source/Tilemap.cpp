#include "Tilemap.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/util/StringCast.h>
#include "GameWorld.h"

const std::vector<Tilemap::Tile>& Tilemap::GetTiles() const
{
	return myTiles;
}

void Tilemap::Read(const nlohmann::json& someData)
{
	auto engine = Tga::Engine::GetInstance();
	myTransform = GetEntity()->GetComponent<Transform>();

	myOrderInLayer = someData["OrderInLayer"];
	for (std::string spritePath : someData["Sprites"])
	{
		Tga::SpriteSharedData shared;
		std::wstring texturePath = string_cast<std::wstring>(spritePath);
		shared.myTexture = engine->GetTextureManager().GetTexture(texturePath.c_str());
		mySharedDatas.push_back(shared);
	}
	for (auto& sprite : someData["TileSprites"])
	{
		size_t spriteIndex = sprite["SpriteIndex"];
		Tga::Sprite2DInstanceData instance;
		instance.myTextureRect.myStartX = sprite["MinX"];
		instance.myTextureRect.myStartY = sprite["MinY"];
		instance.myTextureRect.myEndX = sprite["MaxX"];
		instance.myTextureRect.myEndY = sprite["MaxY"];
		instance.mySize = { 32, 32 };
		instance.myPivot = { 0.0f, 1.f };
		myTileSprites.push_back(TileSprite{ spriteIndex, instance });
	}
	for (auto& tile : someData["Tiles"])
	{
		myTiles.push_back(Tile{ tile[2], { tile[0], tile[1] } });
	}
}

void Tilemap::Render(Tga::GraphicsEngine& aGraphicsEngine)
{
	auto gameWorld = GameWorld::GetInstance();
	auto& renderBuffer = gameWorld->GetRenderBuffer();
	cu::Vector3<float> position = myTransform.lock()->GetWorldPosition();

	for (const Tile& tile : myTiles)
	{
		TileSprite& tileSprite = myTileSprites[tile.TileSpriteIndex];
		Tga::Sprite2DInstanceData instance = tileSprite.SpriteInstance;
		instance.myPosition = Tga::Vector2f(position.ConvertToVector2XY() + cu::Vector2<float>{ static_cast<float>(tile.Position.x * 32.f), static_cast<float>(tile.Position.y * 32) });
		renderBuffer.Push(mySharedDatas[tileSprite.SharedIndex], instance, myOrderInLayer);
	}
	aGraphicsEngine;
}
