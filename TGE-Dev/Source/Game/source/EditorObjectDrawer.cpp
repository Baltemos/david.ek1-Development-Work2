#include "EditorObjectDrawer.h"
#include "JsonUtil.h"
#include "GameWorld.h"

void EditorObjectDrawer::Draw(std::shared_ptr<EditorEntity> aEntity)
{
	aEntity;

	//const ComponentRegistry& registry = GameWorld::GetInstance()->GetComponentRegistry();

	//const EntityTemplate& tEntity = aEntity->GetTemplate();
	//std::shared_ptr<Transform> transform = aEntity->GetEntity()->GetComponent<Transform>();

	//for (const ComponentTemplate& comp : tEntity.GetComponents())
	//{
	//	//if (comp.Type == "RectangleCollider")
	//	//{
	//	//	DrawRectangleCollider(transform, JsonMerge(registry.GetEntry(comp.Type).DefaultData, JsonMerge(comp.Defaults, comp.Overrides, true)));
	//	//}
	//	if (comp.Type == "CircleCollider")
	//	{
	//		DrawCircleCollider(transform, JsonMerge(registry.GetEntry(comp.Type).DefaultData, JsonMerge(comp.Defaults, comp.Overrides, true)));
	//	}
	//}
}

void EditorObjectDrawer::DrawCircleCollider(std::shared_ptr<Transform> aEntity, const nlohmann::json& aData)
{
	std::shared_ptr<Transform> transform = aEntity->GetEntity()->GetComponent<Transform>();
	RenderBuffer* renderBuffer = transform->GetSpaceRenderBuffer();
	cu::Vector3<float> worldPos = transform->GetWorldPosition();

	static constexpr const size_t pointCount = 64;
	static constexpr const float TwoPi = 6.28318530718f;
	static constexpr float angleDelta = TwoPi / static_cast<float>(pointCount);

	float radius = aData["Bounds"]["Radius"];
	cu::Vector3<float> offset;
	offset.x = aData["Bounds"]["Offset"][0];
	offset.y = aData["Bounds"]["Offset"][1];
	Tga::Vector3f lastPos = Tga::Vector3f(cu::Vector3<float>(radius, 0.f, 0.f) + worldPos + offset);

	float angle = 0.f;
	for (size_t ind = 0; ind < pointCount; ind++)
	{
		angle += angleDelta;
		Tga::LinePrimitive line;
		line.color = { 1.f, 0.f, 1.f, 1.f };
		line.fromPosition = lastPos;
		lastPos = Tga::Vector3f(cu::Vector3<float>(std::cos(angle), std::sin(angle), 0.f) * radius + worldPos + offset);
		line.toPosition = lastPos;

		renderBuffer->Push(line, 1000);
	}
}

void EditorObjectDrawer::DrawRectangleCollider(std::shared_ptr<Transform> aEntity, const nlohmann::json& aData)
{
	std::shared_ptr<Transform> transform = aEntity->GetEntity()->GetComponent<Transform>();
	RenderBuffer* renderBuffer = transform->GetSpaceRenderBuffer();
	cu::Vector3<float> worldPos = transform->GetWorldPosition();

	Tga::Vector4f color = { 1.f, 0.f, 1.f, 1.f };

	auto& dMin = aData["Bounds"]["Min"];
	auto& dMax = aData["Bounds"]["Max"];
	Tga::Vector3f max;
	Tga::Vector3f min;
	max.x = dMax[0];
	max.y = dMax[1];
	min.x = dMin[0];
	min.y = dMin[1];

	min += Tga::Vector3f(worldPos);
	max += Tga::Vector3f(worldPos);

	Tga::Vector3f sizeY = { 0, max.y - min.y, 0 };

	Tga::LinePrimitive l1 { color, min, (min + sizeY) };
	Tga::LinePrimitive l2 { color, (min + sizeY), max };
	Tga::LinePrimitive l3 { color, max, max - sizeY };
	Tga::LinePrimitive l4 { color, max - sizeY, min };

	renderBuffer->Push(l1, 1000);
	renderBuffer->Push(l2, 1000);
	renderBuffer->Push(l3, 1000);
	renderBuffer->Push(l4, 1000);
}
