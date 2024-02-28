#pragma once
#include "EditorEntity.h"

class EditorObjectDrawer
{
public:
	void Draw(std::shared_ptr<EditorEntity> aEntity);
private:
	void DrawCircleCollider(std::shared_ptr<Transform> aEntity, const nlohmann::json& aData);
	void DrawRectangleCollider(std::shared_ptr<Transform> aEntity, const nlohmann::json& aData);
};