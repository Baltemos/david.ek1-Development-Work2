#pragma once
#include "EntityTemplate.h"
#include "Component.h"
#include "ClickableObject.h"
#include "EditorRegistry.h"
#include "ComponentRegistry.h"
#include <imgui/imgui.h>

class EditorEntity : public Component
{
public:
	EditorEntity();
	~EditorEntity() = default;

	void SetTemplate(const EntityTemplate2& aTemplate);
	const EntityTemplate2& GetTemplate() const;

	bool IsPrefabInstance() const;

	void MakePrefab();
	void OverwritePrefab();
	void MakeIndependent();

	void Reload();

	void AddComponent(const std::string& aType, const nlohmann::json& aOverrides = {});

	bool TrySetPosition(const cu::Vector3<float>& aPosition);

	void ImGui_ShowObjectEditor(bool* aOpen);

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float) override;
private:
	void ImGui_ShowComponentPopup();
	void ImGui_ShowPrefabNameInputPopup();
	void SetVisible(bool aVisible);
	bool DisplayJson(const std::string& aValuePath, const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverrides, const nlohmann::json& aEditorInfo);

	void UpdateVisuals();
	void UpdateComponentVisuals(const ComponentTemplate& aComponent, const ComponentRegistry::Entry& aEntry);

	bool CheckSavePrefab(const std::string& aPath);

	std::weak_ptr<Transform> myTransform;
	std::weak_ptr<SpriteRenderer2D> myRenderer;

	EntityTemplate2 myTemplate;
	std::string myFile;

	EditorRegistry* myEditorRegistry;
	ComponentRegistry* myComponentRegistry;
	cu::InputHandler* myInputHandler;

	//void onClick(EditorObject*);
};