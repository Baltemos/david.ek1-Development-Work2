#include "EditorRectBounds.h"
#include "EditorEntity.h"
#include "Entity.h"

bool EditorRectBounds::OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	const nlohmann::json value = GetValue(GetDefaultValue(), aOverride);

	std::shared_ptr<Transform> transform = GetEditorEntity()->GetEntity()->GetComponent<Transform>();
	RenderBuffer* renderBuffer = transform->GetSpaceRenderBuffer();
	cu::Vector3<float> worldPos = transform->GetWorldPosition();

	auto& j_color = GetData().at("Color");
	Tga::Vector4f color;
	color.x = j_color[0];
	color.y = j_color[1];
	color.z = j_color[2];
	color.w = j_color[3];

	auto& dMin = value.at("Min");
	auto& dMax = value.at("Max");
	Tga::Vector3f max;
	Tga::Vector3f min;
	max.x = dMax.at(0);
	max.y = dMax.at(1);
	min.x = dMin.at(0);
	min.y = dMin.at(1);

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

	return EditObject(GetDefaultValue(), aOverride, aEditorRegistry);
}
