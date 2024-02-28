#pragma once
#include "Component.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include <CommonUtilities/Vector.hpp>
#include "Transform.h"

namespace cu = CommonUtilities;

class Tilemap : public Component
{
public:
	struct TileSprite
	{
		size_t SharedIndex;
		Tga::Sprite2DInstanceData SpriteInstance;
	};
	struct Tile
	{
		size_t TileSpriteIndex;
		cu::Vector2<int> Position;
	};

	const std::vector<Tile>& GetTiles() const;

	Tilemap();
	~Tilemap();

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Render(Tga::GraphicsEngine& aGraphicsEngine) override;
private:
	std::weak_ptr<Transform> myTransform;

	Tga::RenderTarget myRenderTarget;
	cu::Vector2<unsigned int> myRenderSize;

	Tga::Sprite2DInstanceData myInstance;

	std::vector<Tga::SpriteSharedData> mySharedDatas;
	std::vector<TileSprite> myTileSprites;
	std::vector<Tile> myTiles;
	float myOrderInLayer;
};