#pragma once
#include <string>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/sprite/sprite.h>
#include <set>
#include <unordered_map>
#include <queue>
#include <CommonUtilities/Matrix4x4.hpp>

namespace cu = CommonUtilities;

class RenderBuffer
{
public:
	struct Sprite2DEntry
	{
		Tga::Sprite2DInstanceData Instance;
		Tga::SpriteSharedData Shared;
		//std::string Sprite;
		float ZPosition;
	};

	RenderBuffer() = default;
	~RenderBuffer() = default;

	//void Push(Tga::Sprite2DInstanceData& anInstance, const std::string& anID, float aZPosition);
	void Push(const Tga::SpriteSharedData& aShared, Tga::Sprite2DInstanceData& anInstance, float aZPosition);
	void Push(Tga::Sprite3DInstanceData& anInstance, const std::string& anID);
	void Clear();

	void Render(const cu::Matrix4x4<float>& aCameraMatrix);

private:
	void Render2DSprites(const cu::Matrix4x4<float>& aCameraMatrix);
	void Render3DSprites(const cu::Matrix4x4<float>& aCameraMatrix);

	std::vector<Sprite2DEntry> mySprite2DEntries;

	std::set<std::string> mySprite3DSpriteIDs;
	std::unordered_map<std::string, std::vector<Tga::Sprite3DInstanceData>> mySprite3DEntries;
};

static bool operator<(const RenderBuffer::Sprite2DEntry& aLhs, const RenderBuffer::Sprite2DEntry& aRhs);
