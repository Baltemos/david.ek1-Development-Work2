#pragma once
#include "EntityTemplate.h"
#include "Component.h"
#include "ClickableSprite.h"
#include "EditorRegistry.h"
#include "ComponentRegistry.h"
#include <imgui/imgui.h>

class EditorEntity : public Component
{
public:
	//static Event<std::shared_ptr<EditorEntity>> OnClickEntity;
	static std::shared_ptr<EditorEntity> CreateEditorObject();
	EditorEntity();
	~EditorEntity();

	void SetTemplate(const EntityTemplate& aTemplate);
	const EntityTemplate& GetTemplate() const;

	bool IsPrefabInstance() const;

	//void MakePrefab();
	//void OverwritePrefab();
	//void MakeIndependent();

	void Reload();

	void AddComponent(const std::string& aType, const nlohmann::json& aOverrides = {});

	bool TrySetPosition(const cu::Vector3<float>& aPosition);

	void ImGui_ObjectEditor();

	void SetShowing(bool aShowing);
	bool GetShowing() const;

	void UpdateVisuals();
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float) override;
private:
	void ImGui_ShowComponentPopup();
	//void ImGui_ShowPrefabNameInputPopup();
	void SetVisible(bool aVisible);
	bool DisplayJson(const std::string& aValuePath, const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverrides, const nlohmann::json& aEditorInfo);

	void UpdateComponentVisuals(const ComponentTemplate& aComponent, const ComponentRegistry::Entry& aEntry);

	//bool CheckSavePrefab(const std::string& aPath);

	std::weak_ptr<Transform> myTransform;
	std::weak_ptr<SpriteRenderer2D> myRenderer;

	bool myIsShowing;

	EntityTemplate myEditorEntity;
	std::string myFile;

	EditorRegistry* myEditorRegistry;
	ComponentRegistry* myComponentRegistry;
	cu::InputHandler* myInputHandler;

	//void onClick(EditorObject*);
};