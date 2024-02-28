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
	Tga::Vector4f color = { 1.f, 0.f, 0.f, 1.f };

	Tga::Vector3f max = Tga::Vector3f(cu::Vector3<float>(GetWorldBounds().Max, 0.f));
	Tga::Vector3f min = Tga::Vector3f(cu::Vector3<float>(GetWorldBounds().Min, 0.f));

	Tga::Vector3f sizeY = { 0, max.y - min.y, 0 };

	Tga::LinePrimitive l1 { color, min, (min + sizeY) };
	Tga::LinePrimitive l2 { color, (min + sizeY), max };
	Tga::LinePrimitive l3 { color, max, max - sizeY };
	Tga::LinePrimitive l4 { color, max - sizeY, min };

	RenderBuffer* renderBuffer = myTransform.lock()->GetSpaceRenderBuffer();

	renderBuffer->Push(l1, 1000);
	renderBuffer->Push(l2, 1000);
	renderBuffer->Push(l3, 1000);
	renderBuffer->Push(l4, 1000);
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