#pragma once
#include <string>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/sprite/sprite.h>
#include <set>
#include <unordered_map>
#include <queue>
#include <CommonUtilities/Matrix4x4.hpp>
#include <tge/primitives/CustomShape.h>
#include <tge/primitives/LinePrimitive.h>

namespace cu = CommonUtilities;

class RenderCommand
{
public:
	virtual ~RenderCommand() = default;

	virtual void Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine) = 0;

	void SetLayer(float aLayer);
	float GetLayer() const;

	bool operator<(const RenderCommand& aRhs);
private:
	float myLayer;
};

class Sprite2DRenderCommand : public RenderCommand
{
public:
	Sprite2DRenderCommand(const Tga::SpriteSharedData& aShared, Tga::Sprite2DInstanceData& anInstance, float aLayer);

	virtual void Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine) override;
private:
	Tga::SpriteSharedData myShared;
	Tga::Sprite2DInstanceData myInstance;
};

class CustomShape2DRenderCommand : public RenderCommand
{
public:
	CustomShape2DRenderCommand(const Tga::CustomShape2D& aCustomShape, float aLayer);

	virtual void Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine) override;
private:
	Tga::CustomShape2D myShape;
};

class LineRenderCommand : public RenderCommand
{
public:
	LineRenderCommand(const Tga::LinePrimitive& aLine, float aLayer);

	virtual void Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine) override;
private:
	Tga::LinePrimitive myLine;
};

class MultiLineRenderCommand : public RenderCommand
{
public:
	MultiLineRenderCommand(const Tga::LineMultiPrimitive& aLine, float aLayer);

	virtual void Render(const cu::Matrix4x4<float>& aCameraMatrix, Tga::GraphicsEngine& aGraphicsEngine) override;
private:
	Tga::LineMultiPrimitive myLines;
};

class RenderBuffer
{
public:
	RenderBuffer();
	~RenderBuffer();

	void SetEnabled(bool aEnabled);
	bool GetEnabled() const;

	void Push(const Tga::SpriteSharedData& aShared, Tga::Sprite2DInstanceData& anInstance, float aLayer);
	void Push(const Tga::CustomShape2D& aCustomShape, float aLayer);
	void Push(const Tga::LinePrimitive& aLine, float aLayer);
	void Push(const Tga::LineMultiPrimitive& aLines, float aLayer);
	void Clear();

	void Render(const cu::Matrix4x4<float>& aCameraMatrix);

private:
	std::vector<RenderCommand*> myRenderCommands;
	bool myIsEnabled;
};
