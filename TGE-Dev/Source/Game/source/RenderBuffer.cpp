#include "RenderBuffer.h"
#include <tge/graphics/GraphicsEngine.h>
#include "GameWorld.h"
#include <tge/sprite/sprite.h>
#include <tge/drawers/SpriteDrawer.h>

//void RenderBuffer::Push(Tga::Sprite2DInstanceData& anInstance, const std::string& anID, float aZPosition)
//{
//	mySprite3DSpriteIDs.emplace(anID);
//	mySprite2DEntries.push_back(Sprite2DEntry{ anInstance, anID, aZPosition });
//}

void RenderBuffer::Push(const Tga::SpriteSharedData& aShared, Tga::Sprite2DInstanceData& anInstance, float aZPosition)
{
	mySprite2DEntries.push_back(Sprite2DEntry{ anInstance, aShared, aZPosition });
}

void RenderBuffer::Push(Tga::Sprite3DInstanceData& anInstance, const std::string& anID)
{
	mySprite3DSpriteIDs.emplace(anID);
	mySprite3DEntries[anID].push_back(anInstance);
}

void RenderBuffer::Clear()
{
	mySprite3DSpriteIDs.clear();
	mySprite2DEntries.clear();
	mySprite3DEntries.clear();
}

void RenderBuffer::Render(const cu::Matrix4x4<float>& aCameraMatrix)
{
	Render3DSprites(aCameraMatrix);
	Render2DSprites(aCameraMatrix);
}

void RenderBuffer::Render2DSprites(const cu::Matrix4x4<float>& aCameraMatrix)
{
	Tga::GraphicsEngine& graphicsEngine = Tga::Engine::GetInstance()->GetGraphicsEngine();
	Tga::SpriteDrawer& spriteDrawer = graphicsEngine.GetSpriteDrawer();
	//SpriteManager& spriteManager = GameWorld::GetInstance()->GetSpriteManager();

	std::sort(mySprite2DEntries.begin(), mySprite2DEntries.end());
	for (Sprite2DEntry& entry : mySprite2DEntries)
	{
		//Tga::SpriteSharedData shared = spriteManager.Get(entry.Sprite);
		Tga::SpriteSharedData shared = entry.Shared;
		Tga::Sprite2DInstanceData instance = entry.Instance;
		cu::Vector4<float> position(instance.myPosition.x, instance.myPosition.y, 0, 1);
		position = position * aCameraMatrix;
		instance.myPosition.x = std::floor(position.x);
		instance.myPosition.y = std::floor(position.y);
		instance.mySize.x *= aCameraMatrix.GetRowVectors()[0].Length();
		instance.mySize.y *= aCameraMatrix.GetRowVectors()[1].Length();
		instance.myRotation = instance.myRotation - aCameraMatrix.GetEulerAnglesZYX().z;
		spriteDrawer.Draw(shared, instance);
	}
}

void RenderBuffer::Render3DSprites(const cu::Matrix4x4<float>& aCameraMatrix)
{
	Tga::GraphicsEngine& graphicsEngine = Tga::Engine::GetInstance()->GetGraphicsEngine();
	Tga::SpriteDrawer& spriteDrawer = graphicsEngine.GetSpriteDrawer();
	SpriteManager& spriteManager = GameWorld::GetInstance()->GetSpriteManager();
	for (const std::string& id : mySprite3DSpriteIDs)
	{
		Tga::SpriteBatchScope batch = spriteDrawer.BeginBatch(spriteManager.Get(id));

		auto sprite3DEntries = mySprite3DEntries.find(id);

		if (sprite3DEntries != mySprite3DEntries.end())
		{
			Tga::Matrix4x4f matrix;
			for (int row = 1; row < 5; row++)
			{
				for (int column = 1; column < 5; column++)
				{
					matrix(row, column) = aCameraMatrix(row, column);
				}
			}
			for (Tga::Sprite3DInstanceData instance : sprite3DEntries->second)
			{
				instance.myTransform *= matrix;
				batch.Draw(instance);
			}
		}
	}
}

bool operator<(const RenderBuffer::Sprite2DEntry& aLhs, const RenderBuffer::Sprite2DEntry& aRhs)
{
	return aLhs.ZPosition < aRhs.ZPosition;
}
