#pragma once
#include <nlohmann/json.hpp>

class EditorRegistry;
class EditorEntity;

class ValueEditor
{
public:
	ValueEditor() = default;
	~ValueEditor() = default;

	static constexpr char* ArrayItemName = "item";

	void Init(const std::string& aContainerPath, const std::string& aKey, const nlohmann::json& aDefault, const nlohmann::json& aComponentEditorData, const nlohmann::json& aEditorData, const nlohmann::json* aRootValue, std::shared_ptr<EditorEntity> aEditorEntity);
	void Init(const std::string& aContainerPath, const std::string& aKey, const nlohmann::json& aDefault, const nlohmann::json& aComponentEditorData, const nlohmann::json* aRootValue, std::shared_ptr<EditorEntity> aEditorEntity);
	void Init(const ValueEditor& aParent, const std::string& aKey, const nlohmann::json& aDefaultValue);
	
	const std::string& GetContainerPath() const;
	std::string ValuePath() const;
	const std::string& GetKey() const;
	const nlohmann::json& GetDefaultValue() const;
	const nlohmann::json& GetComponentEditorData() const;
	const nlohmann::json& GetData() const;
	const nlohmann::json* GetRootValue() const;
	std::shared_ptr<EditorEntity> GetEditorEntity() const;

	virtual bool OnEditValue(nlohmann::json& aOverride, EditorRegistry& aEditorRegistry);
protected:
	nlohmann::json GetValue(const nlohmann::json& aDefault, nlohmann::json& aOverride) const;
	std::shared_ptr<ValueEditor> GetEditor(const std::string& aEditorKey, const nlohmann::json& aDefaultData, EditorRegistry& aEditorRegistry) const;

	bool EditObject(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride, EditorRegistry& aEditorRegistry);
	bool EditArray(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride, EditorRegistry& aEditorRegistry);
	bool EditString(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride);
	bool EditInteger(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride);
	bool EditFloat(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride);
	bool EditBool(const nlohmann::json& aDefaultValue, nlohmann::json& aOverride);
	bool HandleNull();

private:
	std::string myContainerPath;
	std::string myKey;
	std::weak_ptr<EditorEntity> myEditorEntity;
	const nlohmann::json* myRootValue;
	nlohmann::json myDefaultValue;
	nlohmann::json myComponentEditorData;
	nlohmann::json myData;
};