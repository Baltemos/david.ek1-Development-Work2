#include "Tilemap.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/util/StringCast.h>
#include "GameWorld.h"
#include <CommonUtilities/AABB2D.hpp>
#include <tge/graphics/GraphicsStateStack.h>
#include <tge/graphics/DX11.h>

namespace cu = CommonUtilities;

const std::vector<Tilemap::Tile>& Tilemap::GetTiles() const
{
	return myTiles;
}

Tilemap::Tilemap()
{
}

Tilemap::~Tilemap()
{
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

	cu::AABB2D<int> myTilesRect({ INT_MAX, INT_MAX }, { INT_MIN, INT_MIN });
	for (auto& tileData : someData["Tiles"])
	{
		Tile tile;
		tile.Position = cu::Vector2<int>(tileData[0], tileData[1]);
		tile.TileSpriteIndex = tileData[2];
		myTiles.push_back(tile);

		myTilesRect.Max.x = (std::max)(myTilesRect.Max.x, tile.Position.x + 1);
		myTilesRect.Max.y = (std::max)(myTilesRect.Max.y, tile.Position.y + 1);
		myTilesRect.Min.x = (std::min)(myTilesRect.Min.x, tile.Position.x);
		myTilesRect.Min.y = (std::min)(myTilesRect.Min.y, tile.Position.y);
	}

	unsigned int width = static_cast<unsigned int>(myTilesRect.Max.x - myTilesRect.Min.x);
	unsigned int height = static_cast<unsigned int>(myTilesRect.Max.y - myTilesRect.Min.y);
	myInstance.myPivot.x = static_cast<float>(-myTilesRect.Min.x) / static_cast<float>(width);
	myInstance.myPivot.y = 1.f - static_cast<float>(-myTilesRect.Min.y) / static_cast<float>(height);
	myRenderSize.x = 32 * width;
	myRenderSize.y = 32 * height;

	auto& graphicsEngine = Tga::Engine::GetInstance()->GetGraphicsEngine();
	auto& graphicsStateStack = graphicsEngine.GetGraphicsStateStack();
	auto& drawer = graphicsEngine.GetSpriteDrawer();
	graphicsStateStack.Push();
	Tga::Camera camera;
	camera.SetOrtographicProjection(myTilesRect.Min.x * 32.f, myTilesRect.Max.x * 32.f, myTilesRect.Min.y * 32.f, myTilesRect.Max.y * 32.f, -1, 1);
	graphicsStateStack.SetCamera(camera);

	myRenderTarget = Tga::RenderTarget::Create({ myRenderSize.x, myRenderSize.y });
	myRenderTarget.SetAsActiveTarget();
	myRenderTarget.Clear();

	for (Tile& tile : myTiles)
	{
		TileSprite& tileSprite = myTileSprites[tile.TileSpriteIndex];
		Tga::Sprite2DInstanceData instance = tileSprite.SpriteInstance;
		instance.myPosition = Tga::Vector2f(cu::Vector2<float>{ static_cast<float>(tile.Position.x * 32.f), static_cast<float>(tile.Position.y * 32) });
		drawer.Draw(mySharedDatas[tileSprite.SharedIndex], instance);
	}


	graphicsStateStack.Pop();
}

void Tilemap::Render(Tga::GraphicsEngine& aGraphicsEngine)
{
	auto gameWorld = GameWorld::GetInstance();
	auto& renderBuffer = gameWorld->GetRenderBuffer();
	cu::Vector3<float> position = myTransform.lock()->GetWorldPosition();

	Tga::SpriteSharedData shared;
	shared.myTexture = &myRenderTarget;

	myInstance.myPosition = Tga::Vector2f(position.ConvertToVector2XY());
	myInstance.mySize = Tga::Vector2ui(myRenderSize);

	renderBuffer.Push(shared, myInstance, myOrderInLayer);

	//for (const Tile& tile : myTiles)
	//{
	//	TileSprite& tileSprite = myTileSprites[tile.TileSpriteIndex];
	//	Tga::Sprite2DInstanceData instance = tileSprite.SpriteInstance;
	//	instance.myPosition = Tga::Vector2f(position.ConvertToVector2XY() + cu::Vector2<float>{ static_cast<float>(tile.Position.x * 32.f), static_cast<float>(tile.Position.y * 32) });
	//	renderBuffer.Push(mySharedDatas[tileSprite.SharedIndex], instance, myOrderInLayer);
	//}
	aGraphicsEngine;
}
