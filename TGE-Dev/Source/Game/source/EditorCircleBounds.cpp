#include "EditorCircleBounds.h"
#include "Entity.h"
#include "EditorEntity.h"

bool EditorCircleBounds::OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry)
{
	const nlohmann::json value = GetValue(GetDefaultValue(), aOverride);

	std::shared_ptr<Transform> transform = GetEditorEntity()->GetEntity()->GetComponent<Transform>();
	RenderBuffer* renderBuffer = transform->GetSpaceRenderBuffer();
	cu::Vector3<float> worldPos = transform->GetWorldPosition();

	static constexpr const size_t pointCount = 64;
	static constexpr const float TwoPi = 6.28318530718f;
	static constexpr float angleDelta = TwoPi / static_cast<float>(pointCount);

	auto& j_color = GetData().at("Color");
	Tga::Vector4f color;
	color.x = j_color[0];
	color.y = j_color[1];
	color.z = j_color[2];
	color.w = j_color[3];

	float radius = value.at("Radius");
	cu::Vector3<float> offset;
	offset.x = value.at("Offset").at(0);
	offset.y = value.at("Offset").at(1);
	Tga::Vector3f lastPos = Tga::Vector3f(cu::Vector3<float>(radius, 0.f, 0.f) + worldPos + offset);

	float angle = 0.f;
	for (size_t ind = 0; ind < pointCount; ind++)
	{
		angle += angleDelta;
		Tga::LinePrimitive line;
		line.color = color;
		line.fromPosition = lastPos;
		lastPos = Tga::Vector3f(cu::Vector3<float>(std::cos(angle), std::sin(angle), 0.f) * radius + worldPos + offset);
		line.toPosition = lastPos;

		renderBuffer->Push(line, 1000);
	}

	return EditObject(GetDefaultValue(), aOverride, aEditorRegistry);
}
