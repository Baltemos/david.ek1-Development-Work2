#pragma once
#include <unordered_map>
#include <string>
#include "ValueEditor.h"

class EditorRegistry
{
public:
	EditorRegistry() = default;
	~EditorRegistry() = default;

	template<typename _ValueEditorTy>
	void Register(const std::string& aValueType);

	ValueEditor* Get(const std::string& aValueType);

	ValueEditor* GetDefault();

private:
	std::unordered_map<std::string, std::shared_ptr<ValueEditor>> myEditors;
	ValueEditor myDefaultEditor;
};

template<typename _ValueEditorTy>
inline void EditorRegistry::Register(const std::string& aValueType)
{
	myEditors.emplace(aValueType, std::make_shared<_ValueEditorTy>());
}
