#pragma once
#include <unordered_map>
#include <string>
#include "ValueEditor.h"
#include <functional>

class EditorRegistry
{
public:
	EditorRegistry() = default;
	~EditorRegistry() = default;

	template<typename _ValueEditorTy>
	void Register(const std::string& aValueType);

	std::shared_ptr<ValueEditor> Get(const std::string& aValueType);

	std::shared_ptr<ValueEditor> GetDefault();

private:
	std::unordered_map<std::string, std::function<std::shared_ptr<ValueEditor>()>> myEditors;
	ValueEditor myDefaultEditor;
};

template<typename _ValueEditorTy>
inline void EditorRegistry::Register(const std::string& aValueType)
{
	myEditors.emplace(aValueType, []() { return std::dynamic_pointer_cast<ValueEditor>(std::make_shared<_ValueEditorTy>()); });
}
