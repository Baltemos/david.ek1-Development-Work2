#pragma once
#include "Component.h"
#include <tge/settings/settings.h>
#include <set>

class Entity;

class ComponentRegistry
{
public:
	ComponentRegistry() = default;
	~ComponentRegistry() = default;

	struct Entry
	{
		std::function<std::shared_ptr<Component>()> Constructor;
		nlohmann::json DefaultData;
		nlohmann::json EditorInfo;
	};

	template<typename _ComponentTy>
	void RegisterD(const std::string& aName, const nlohmann::json& aDefaultData, const nlohmann::json& aEditorInfo);
	template<typename _ComponentTy>
	void Register(const std::string& aName, const std::string& aDefaultDataPath, const std::string& aEditorInfoPath);

	const Entry& GetEntry(const std::string& aName) const;

	const std::set<std::string>& GetRegistered() const;

private:
	std::unordered_map<std::string, Entry> myEntries;
	std::set<std::string> myRegistered;
};

template<typename _ComponentTy>
inline void ComponentRegistry::RegisterD(const std::string& aName, const nlohmann::json& aDefaultData, const nlohmann::json& aEditorInfo)
{
	myRegistered.emplace(aName);
	myEntries.emplace(aName, Entry{ &std::make_shared<_ComponentTy>, aDefaultData, aEditorInfo });
}

template<typename _ComponentTy>
inline void ComponentRegistry::Register(const std::string& aName, const std::string& aDefaultDataPath, const std::string& aEditorInfoPath)
{
	std::ifstream file(Tga::Settings::ResolveGameAssetPath(aDefaultDataPath));
	nlohmann::json data = {};
	if (file.is_open())
	{
		file >> data;
	}
	std::ifstream editorFile(Tga::Settings::ResolveGameAssetPath(aEditorInfoPath));
	nlohmann::json editorInfo = {};
	if (editorFile.is_open())
	{
		editorFile >> editorInfo;
	}
	RegisterD<_ComponentTy>(aName, data, editorInfo);
}
