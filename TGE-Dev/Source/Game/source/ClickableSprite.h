#pragma once
#include "Component.h"
#include "Event.h"
#include <CommonUtilities/AABB2D.hpp>
#include "Transform.h"
#include "SpriteRenderer2D.h"
#include <CommonUtilities/InputHandler.h>

namespace cu = CommonUtilities;

class ClickableSprite : public Component
{
public:
	Event<ClickableSprite*>& GetOnClick();

	cu::AABB2D<float> GetWorldBounds() const;

protected:
	virtual void Read(const nlohmann::json& someDate);
	virtual void Start() override;
	virtual void Update(float aTimeDelta) override;

private:
	cu::InputHandler* myInputHandler;

	Event<ClickableSprite*> myOnClick;

	std::weak_ptr<Transform> myTransform;
	std::weak_ptr<SpriteRenderer2D> myRenderer;

	cu::AABB2D<float> myBounds;
};