#pragma once
#include "Component.h"
#include <tge/sprite/sprite.h>
#include "Transform.h"

class SpriteRenderer2D : public Component
{
public:
	SpriteRenderer2D() = default;
	~SpriteRenderer2D() = default;

	virtual void Read(const nlohmann::json& someData) override;

	const Tga::Sprite2DInstanceData& GetInstance() const;
	void SetInstance(const Tga::Sprite2DInstanceData& aInstance);

protected:
	virtual void Start() override;
	virtual void Render(Tga::GraphicsEngine& aGraphicsEngine) override;

private:
	std::weak_ptr<Transform> myTransform;

	float myOrderInLayer;
	std::string mySpritePath;
	Tga::Sprite2DInstanceData myInstance;
	Tga::SpriteSharedData myShared;
};