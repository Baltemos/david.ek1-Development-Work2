#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "CollisionGrid.h"
#include "Entity.h"
#include "tge/engine.h"
#include <tge/drawers/DebugDrawer.h>
#include "GameWorld.h"

void RectangleCollider::Read(const nlohmann::json& someData)
{
	SetType(ColliderType::eRectangle);

	myTransform = GetEntity()->GetComponent<Transform>();

	auto& bounds = someData["Bounds"];
	myBounds.Min.x = bounds["Min"][0];
	myBounds.Min.y = bounds["Min"][1];
	myBounds.Max.x = bounds["Max"][0];
	myBounds.Max.y = bounds["Max"][1];

	Collider::Read(someData);
}

void RectangleCollider::Update(float)
{
	UpdateBounds();

#ifdef _DEBUG
	Tga::Sprite2DInstanceData instance;
	instance.myPosition = Tga::Vector2f(GetWorldBounds().Min);
	instance.myPivot = { 0, 1 };
	instance.mySize = Tga::Vector2f(GetWorldBounds().Max - GetWorldBounds().Min);
	instance.myColor = { 1, 0, 0, 0.5f };
	GameWorld::GetInstance()->GetRenderBuffer().Push({}, instance, 100);
#endif
}

void RectangleCollider::UpdateBounds()
{
	std::shared_ptr<Transform> transform = myTransform.lock();

	cu::Vector2<float> translation = transform->GetWorldPosition().ConvertToVector2XY();

	cu::Vector2<float> min = myBounds.Min + translation;
	cu::Vector2<float> max = myBounds.Max + translation;;

	myWorldBounds.Min = min;
	myWorldBounds.Max = max;

	cu::Vector2<float> center = (max + min) * 0.5f;

	SetBoundingCircle(cu::Circle<float>(center, (center - min).Length()));
}

void RectangleCollider::SetBounds(cu::AABB2D<float> aBounds)
{
	myBounds = aBounds;
}

const cu::AABB2D<float>& RectangleCollider::GetWorldBounds() const
{
	return myWorldBounds;
}