#pragma once
#include "Component.h"
#include <tge/sprite/sprite.h>
#include "Transform.h"

class SpriteRenderer2D : public Component
{
public:
	SpriteRenderer2D();
	~SpriteRenderer2D() = default;

	//Read is placed here to allow the editor to access it.
	virtual void Read(const nlohmann::json& someData) override;

	void SetFlippedX(bool aFlipped);
	void SetFlippedY(bool aFlipped);
	const cu::Vector2<bool>& GetFlipped() const;

	const Tga::Sprite2DInstanceData& GetInstance() const;
	void SetInstance(const Tga::Sprite2DInstanceData& aInstance);
	const Tga::SpriteSharedData& GetShared() const;
	void SetShared(const Tga::SpriteSharedData& aShared);

protected:
	virtual void Start() override;
	virtual void Render(Tga::GraphicsEngine& aGraphicsEngine) override;

private:
	std::weak_ptr<Transform> myTransform;

	float myOrderInLayer;
	cu::Vector2<bool> myIsFlipped;
	std::string mySpritePath;
	Tga::Sprite2DInstanceData myInstance;
	Tga::SpriteSharedData myShared;
};