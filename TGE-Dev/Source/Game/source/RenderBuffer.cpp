#include "RenderBuffer.h"
#include <tge/graphics/GraphicsEngine.h>
#include "GameWorld.h"
#include <tge/sprite/sprite.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/drawers/CustomShapeDrawer.h>
#include <tge/drawers/LineDrawer.h>

//void RenderBuffer::Push(Tga::Sprite2DInstanceData& anInstance, const std::string& anID, float aZPosition)
//{
//	mySprite3DSpriteIDs.emplace(anID);
//	mySprite2DEntries.push_back(Sprite2DEntry{ anInstance, anID, aZPosition });
//}

void RenderCommand::SetLayer(float aLayer)
{
	myLayer = aLayer;
}

float RenderCommand::GetLayer() const
{
	return myLayer;
}

bool RenderCommand::operator<(const RenderCommand& aRhs)
{
	return myLayer < aRhs.myLayer;
}

Sprite2DRenderCommand::Sprite2DRenderCommand(const Tga::SpriteSharedData& aShared, Tga::Sprite2DInstanceData& anInstance, float aLayer)
{
	myShared = aShared;
	myInstance = anInstance;
	SetLayer(aLayer);
}

void Sprite2DRenderCommand::Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine)
{
	Tga::SpriteDrawer& spriteDrawer = aGraphicsEngine.GetSpriteDrawer();

	Tga::SpriteSharedData shared = myShared;
	Tga::Sprite2DInstanceData instance = myInstance;
	cu::Vector4<float> position(instance.myPosition.x, instance.myPosition.y, 0, 1);
	position = position * aCameraMatrix;
	//instance.myPosition.x = std::floor(position.x);
	//instance.myPosition.y = std::floor(position.y);
	instance.myPosition.x = position.x;
	instance.myPosition.y = position.y;
	instance.mySize.x *= aCameraMatrix.GetRowVectors()[0].Length();
	instance.mySize.y *= aCameraMatrix.GetRowVectors()[1].Length();
	instance.myRotation = instance.myRotation - aCameraMatrix.GetEulerAnglesZYX().z;
	spriteDrawer.Draw(shared, instance);
}

CustomShape2DRenderCommand::CustomShape2DRenderCommand(const Tga::CustomShape2D& aCustomShape, float aLayer)
{
	myShape = aCustomShape;
	SetLayer(aLayer);
}

void CustomShape2DRenderCommand::Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine)
{
	Tga::CustomShapeDrawer& shapeDrawer = aGraphicsEngine.GetCustomShapeDrawer();

	Tga::CustomShape2D shape = myShape;
	for (Tga::SCustomPoint& point : shape.myPoints)
	{
		cu::Vector4<float> pos = {
			point.myPosition.x,
			point.myPosition.y,
			0.f,
			1.f
		};
		pos = pos * aCameraMatrix;
		point.myPosition.x = pos.x;
		point.myPosition.y = pos.y;
	}

	shapeDrawer.Draw(shape);
}

LineRenderCommand::LineRenderCommand(const Tga::LinePrimitive& aLine, float aLayer)
{
	myLine = aLine;
	SetLayer(aLayer);
}

void LineRenderCommand::Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine)
{
	Tga::LineDrawer& lineDrawer = aGraphicsEngine.GetLineDrawer();

	Tga::LinePrimitive line = myLine;

	cu::Vector4<float> from = { line.fromPosition.x, line.fromPosition.y, 0, 1.f };
	cu::Vector4<float> to = { line.toPosition.x, line.toPosition.y, 0, 1.f };

	from = from * aCameraMatrix;
	to = to * aCameraMatrix;

	line.fromPosition.x = from.x;
	line.fromPosition.y = from.y;
	line.fromPosition.z = 0.f;

	line.toPosition.x = to.x;
	line.toPosition.y = to.y;
	line.toPosition.z = 0.f;

	lineDrawer.Draw(line);
}

MultiLineRenderCommand::MultiLineRenderCommand(const Tga::LineMultiPrimitive& aLine, float aLayer)
{
	myLines = aLine;
	SetLayer(aLayer);
}

void MultiLineRenderCommand::Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine)
{
	Tga::LineDrawer& lineDrawe = aGraphicsEngine.GetLineDrawer();

	for (size_t ind = 0; ind < myLines.count; ind++)
	{
		Tga::LinePrimitive line;
		line.fromPosition = myLines.fromPositions[ind];
		line.toPosition = myLines.toPositions[ind];
		line.color.x = myLines.colors[ind].myR;
		line.color.y = myLines.colors[ind].myG;
		line.color.z = myLines.colors[ind].myB;
		line.color.w = myLines.colors[ind].myA;

		cu::Vector4<float> from = { line.fromPosition.x, line.fromPosition.y, 0, 1.f };
		cu::Vector4<float> to = { line.toPosition.x, line.toPosition.y, 0, 1.f };

		from = from * aCameraMatrix;
		to = to * aCameraMatrix;

		line.fromPosition.x = from.x;
		line.fromPosition.y = from.y;
		line.fromPosition.z = 0.f;

		line.toPosition.x = to.x;
		line.toPosition.y = to.y;
		line.toPosition.z = 0.f;

		lineDrawe.Draw(line);
	}
}

RenderBuffer::RenderBuffer()
{
	myIsEnabled = true;
}

RenderBuffer::~RenderBuffer()
{
	Clear();
}

void RenderBuffer::SetEnabled(bool aEnabled)
{
	myIsEnabled = aEnabled;
}

bool RenderBuffer::GetEnabled() const
{
	return myIsEnabled;
}

void RenderBuffer::Push(const Tga::SpriteSharedData& aShared, Tga::Sprite2DInstanceData& anInstance, float aZPosition)
{
	myRenderCommands.push_back(new Sprite2DRenderCommand(aShared, anInstance, aZPosition));
}

void RenderBuffer::Push(const Tga::CustomShape2D& aCustomShape, float aZPosition)
{
	myRenderCommands.push_back(new CustomShape2DRenderCommand(aCustomShape, aZPosition));
}

void RenderBuffer::Push(const Tga::LinePrimitive& aLine, float aLayer)
{
	myRenderCommands.push_back(new LineRenderCommand(aLine, aLayer));
}

void RenderBuffer::Push(const Tga::LineMultiPrimitive& aLines, float aLayer)
{
	myRenderCommands.push_back(new MultiLineRenderCommand(aLines, aLayer));
}

void RenderBuffer::Clear()
{
	for (RenderCommand* ptr : myRenderCommands)
	{
		delete ptr;
	}
	myRenderCommands.clear();
}

void RenderBuffer::Render(const cu::Matrix4x4<float>& aCameraMatrix)
{
	if (myIsEnabled)
	{
		Tga::GraphicsEngine& graphicsEngine = Tga::Engine::GetInstance()->GetGraphicsEngine();

		std::sort(myRenderCommands.begin(), myRenderCommands.end(),
			[](RenderCommand* aLhs, RenderCommand* aRhs) {
				return *aLhs < *aRhs;
			}
		);
		for (RenderCommand* command : myRenderCommands)
		{
			command->Render(aCameraMatrix, graphicsEngine);
		}
	}
}
