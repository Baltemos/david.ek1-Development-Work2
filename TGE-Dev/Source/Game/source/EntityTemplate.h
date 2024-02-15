#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "ComponentTemplate.h"
#include <iostream>

class EntityPrefab;

class EntityTemplate2
{
public:
	EntityTemplate2();
	EntityTemplate2(const std::string& aPrefabPath);
	EntityTemplate2(const EntityTemplate2& aEntity);
	~EntityTemplate2();

	EntityTemplate2& operator=(const EntityTemplate2& aEntity);

	void SetName(const std::string& aName);
	const std::string& GetName() const;

	std::vector<ComponentTemplate> GetComponents() const;
	std::vector<ComponentTemplate>& GetRawComponents();
	std::vector<EntityTemplate2> GetChildren() const;
	std::vector<EntityTemplate2>& GetRawChildren();

	const EntityPrefab* GetPrefab() const;

	bool ContainsComponent(const std::string& aType) const;
	void ClearOverrides();

	//If this template is an instance of a prefab. Prefab instances cannot have components or children added to them.
	bool IsPrefabInstance() const;
	EntityTemplate2 GetIndependent() const;

	//Overwrites prefab with this instance. Effectively updating the overrides of the prefab.
	void OverwritePrefab();
	//Saves this template.
	nlohmann::json Save() const;

	void Load(const nlohmann::json& someData);
	void Reload();

	void AddComponent(const std::string& aType, const nlohmann::json& someOverrides = nlohmann::json(nlohmann::detail::value_t::null));

	bool TryAddComponent(const ComponentTemplate& aComponent);
	bool TryEraseComponent(size_t aIndex);
	bool TryAddChild(const EntityTemplate2& aEntity);

private:
	void InitBlankPrefabInstance();

	std::string myName;
	EntityPrefab* myPrefab;
	std::vector<ComponentTemplate> myComponents;
	std::vector<EntityTemplate2> myChildren;
};

class EntityPrefab
{
public:
	void Load(const std::string& aAssetPath);
	void Reload();
	void Overwrite(const EntityTemplate2& aEntity);

	std::vector<ComponentTemplate> GetComponents() const;
	std::vector<ComponentTemplate>& GetRawComponents();
	std::vector<EntityTemplate2> GetChildren() const;

	const EntityTemplate2& GetTemplate() const;

	const std::string& GetPath() const;

private:
	std::string myPath;
	EntityTemplate2 myPrefab;
};

//class EntityPrefab
//{
//public:
//	using Iterator = std::vector<const ComponentTemplate>::iterator;
//	using ConstIterator = std::vector<const ComponentTemplate>::const_iterator;
//
//	Iterator begin();
//	ConstIterator begin() const;
//	Iterator end();
//	ConstIterator end() const;
//
//	const std::vector<const ComponentTemplate> GetComponents() const;
//	const std::vector<const EntityData*> GetChildren() const;
//
//private:
//	std::vector<ComponentTemplate> myDefaults;
//	std::vector<EntityData*> myChildren;
//};
//
//class EntityData
//{
//public:
//	virtual std::vector<ComponentTemplate> GetComponents() const = 0;
//	virtual std::vector<const EntityData*> GetChildren() const = 0;
//	virtual EntityPrefab SaveAsPrefab() const = 0;
//	virtual void Save() = 0;
//
//	void SetName(const std::string& aName);
//	const std::string& GetName() const;
//
//private:
//	std::string myName;
//};
//
//class EntityPrefabInstance : public EntityData
//{
//public:
//	EntityPrefabInstance(const EntityPrefab& aPrefab);
//
//private:
//	EntityPrefab myPrefab;
//};
//
//class EntityTemplate : public EntityData
//{
//public:
//	void AddComponent(const std::string& aType, nlohmann::json someOverrides = nlohmann::json(nlohmann::detail::value_t::null));
//
//private:
//};



//class EntityTemplate
//{
//public:
//	using Iterator = std::vector<ComponentTemplate>::iterator;
//	using ConstIterator = std::vector<ComponentTemplate>::const_iterator;
//
//	EntityTemplate();
//	~EntityTemplate() = default;
//
//	Iterator begin();
//	ConstIterator begin() const;
//	Iterator end();
//	ConstIterator end() const;
//
//	EntityTemplate::Iterator erase(const Iterator& aPosition);
//	EntityTemplate::ConstIterator erase(const ConstIterator& aPosition);
//
//	void ClearOverrides();
//
//	void SetName(const std::string& aName);
//	const std::string& GetName() const;
//
//	bool SetPrefabPath(const std::string& aPath);
//	const std::string& GetPrefabPath() const;
//	bool IsPrefab() const;
//
//	EntityTemplate SumPrefabs() const;
//
//	EntityTemplate Merge(const EntityTemplate& aEntity) const;
//
//	void AddComponent(const std::string& aType, nlohmann::json someOverrides = nlohmann::json(nlohmann::detail::value_t::null));
//
//	bool ContainsComponent(const std::string& aType) const;
//
//	const std::vector<ComponentTemplate>& GetComponents() const;
//	const std::vector<EntityTemplate>& GetChildren() const;
//
//	void Load(const nlohmann::json& someData);
//	nlohmann::json Save() const;
//
//	void Read(std::istream& aStream);
//	void Write(std::ostream& aStream) const;
//
//private:
//	std::string myName;
//	std::string myPrefabPath;
//	std::vector<ComponentTemplate> myComponents;
//	std::vector<EntityTemplate> myChildren;
//};

//static std::istream& operator>>(std::istream& aStream, ComponentTemplate& aEntityTemplate);
//std::ostream& operator<<(std::ostream& aStream, const ComponentTemplate& aEntityTemplate);