#include "ClickableObject.h"
#include "Entity.h"
#include "GameWorld.h"
#include "Camera.h"
#include "SpriteRenderer2D.h"
#include <imgui/imgui.h>

Event<ClickableObject*>& ClickableObject::GetOnClick()
{
	return myOnClick;
}

cu::AABB2D<float> ClickableObject::GetWorldBounds() const
{
	cu::Vector2<float> center = myTransform.lock()->GetWorldPosition().ConvertToVector2XY();
	return cu::AABB2D<float>(center + myBounds.Min, center + myBounds.Max);
}

void ClickableObject::Read(const nlohmann::json& /*someDate*/)
{
	myTransform = GetEntity()->GetComponent<Transform>();
	myRenderer = GetEntity()->GetComponent<SpriteRenderer2D>();
	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();
}

void ClickableObject::Start()
{
}

void ClickableObject::Update(float /*aTimeDelta*/)
{
	std::shared_ptr<SpriteRenderer2D> renderer = myRenderer.lock();
	Tga::Sprite2DInstanceData instance = renderer->GetInstance();
	Tga::Vector2f tgaSize = instance.mySize;
	cu::Vector2<float> size = { (std::max)(8.f, tgaSize.x), (std::max)(8.f, tgaSize.y)};
	cu::Vector2<float> pivot = { instance.myPivot.x, 1.f - instance.myPivot.y };
	
	myBounds.Min = -size * pivot;
	myBounds.Max = size * (cu::Vector2<float>(1.f, 1.f) - pivot);

	if (myInputHandler->IsKeyDown(static_cast<int>(cu::Keys::MOUSELBUTTON)))
	{
		POINT mousePosition = myInputHandler->GetMouseClientPosition();
		cu::Vector2<float> worldPos = Camera::GetActiveCamera()->ScreenToWorld({ static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0 }).ConvertToVector2XY();
		if (GetWorldBounds().IsInside(worldPos))
		{
			myOnClick(this);
		}
	}
}
