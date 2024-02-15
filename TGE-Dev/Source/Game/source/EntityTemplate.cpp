#include "EntityTemplate.h"
#include <fstream>
#include "JsonUtil.h"
#include <tge/settings/settings.h>

void EntityPrefab::Load(const std::string& aAssetPath)
{
	myPath = aAssetPath;
	std::string resolvedPath = Tga::Settings::ResolveGameAssetPath(aAssetPath);
	std::ifstream file(resolvedPath);
	nlohmann::json data;
	file >> data;
	myPrefab.Load(data);
}

void EntityPrefab::Reload()
{
	Load(myPath);
}

void EntityPrefab::Overwrite(const EntityTemplate2& aEntity)
{
	std::string resolvedPath = Tga::Settings::ResolveGameAssetPath(myPath);
	{
		std::ofstream file(resolvedPath);
		file << aEntity.Save();
	}
	Reload();
}

std::vector<ComponentTemplate> EntityPrefab::GetComponents() const
{
	return myPrefab.GetComponents();
}

std::vector<ComponentTemplate>& EntityPrefab::GetRawComponents()
{
	return myPrefab.GetRawComponents();
}

std::vector<EntityTemplate2> EntityPrefab::GetChildren() const
{
	return myPrefab.GetChildren();
}

const EntityTemplate2& EntityPrefab::GetTemplate() const
{
	return myPrefab;
}

const std::string& EntityPrefab::GetPath() const
{
	return myPath;
}

EntityTemplate2::EntityTemplate2()
{
	myPrefab = nullptr;
}
EntityTemplate2::EntityTemplate2(const std::string& aPrefabPath)
{
	myPrefab = new EntityPrefab();
	myPrefab->Load(aPrefabPath);
	InitBlankPrefabInstance();
}
EntityTemplate2::~EntityTemplate2()
{
	if (myPrefab)
	{
		delete myPrefab;
	}
}
EntityTemplate2& EntityTemplate2::operator=(const EntityTemplate2& aEntity)
{
	if (aEntity.myPrefab)
	{
		myPrefab = new EntityPrefab(*aEntity.myPrefab);
	}
	else
	{
		myPrefab = nullptr;
	}
	myComponents = aEntity.myComponents;
	myChildren = aEntity.myChildren;
	myName = aEntity.myName;
	return *this;
}
EntityTemplate2::EntityTemplate2(const EntityTemplate2& aEntity)
{
	*this = aEntity;
}
void EntityTemplate2::SetName(const std::string& aName)
{
	myName = aName;
}
const std::string& EntityTemplate2::GetName() const
{
	return myName;
}
std::vector<ComponentTemplate> EntityTemplate2::GetComponents() const
{
	std::vector<ComponentTemplate> components = myComponents;
	if (myPrefab)
	{
		std::vector<ComponentTemplate> prefabComponents = myPrefab->GetComponents();
		for (size_t ind = 0; ind < components.size(); ind++)
		{
			components[ind].Defaults = JsonMerge(prefabComponents[ind].Defaults, prefabComponents[ind].Overrides, true);
		}
	}
	return components;
}
std::vector<ComponentTemplate>& EntityTemplate2::GetRawComponents()
{
	return myComponents;
}
std::vector<EntityTemplate2> EntityTemplate2::GetChildren() const
{
	std::vector<EntityTemplate2> children = myChildren;
	if (IsPrefabInstance())
	{
		std::vector<EntityTemplate2> prefabChildren = myPrefab->GetChildren();
		children.insert(children.end(), prefabChildren.begin(), prefabChildren.end());
	}
	return children;
}
std::vector<EntityTemplate2>& EntityTemplate2::GetRawChildren()
{
	return myChildren;
}
const EntityPrefab* EntityTemplate2::GetPrefab() const
{
	return myPrefab;
}
bool EntityTemplate2::ContainsComponent(const std::string& aType) const
{
	for (auto& comp : myComponents)
	{
		if (comp.Type == aType)
		{
			return true;
		}
	}
	return false;
}
void EntityTemplate2::ClearOverrides()
{
	for (auto& comp : myComponents)
	{
		comp.Overrides = nlohmann::json(nlohmann::detail::value_t::null);
	}
}
bool EntityTemplate2::IsPrefabInstance() const
{
	return myPrefab != nullptr;
}
EntityTemplate2 EntityTemplate2::GetIndependent() const
{
	EntityTemplate2 tEntity;
	tEntity.myChildren = myChildren;

	std::vector<ComponentTemplate> components = GetComponents();
	for (ComponentTemplate comp : components)
	{
		comp.Overrides = JsonMerge(comp.Defaults, comp.Overrides, true);
		comp.Defaults = nlohmann::json(nlohmann::detail::value_t::null);
		tEntity.TryAddComponent(comp);
	}
	return tEntity;
}
void EntityTemplate2::OverwritePrefab()
{
	EntityTemplate2 overwritten = myPrefab->GetTemplate();

	for (size_t ind = 0; ind < overwritten.myComponents.size(); ind++)
	{
		overwritten.myComponents[ind].Overrides = JsonMerge(overwritten.myComponents[ind].Overrides, myComponents[ind].Overrides, true);
	}

	myPrefab->Overwrite(overwritten);
	ClearOverrides();
}
nlohmann::json EntityTemplate2::Save() const
{
	nlohmann::json data;

	if (IsPrefabInstance())
	{
		data["PrefabPath"] = myPrefab->GetPath();
	}

	data["Name"] = myName;

	nlohmann::json components(nlohmann::detail::value_t::array);
	for (auto& component : myComponents)
	{
		nlohmann::json componentData;
		componentData["Type"] = component.Type;
		componentData["Data"] = component.Overrides;
		components += componentData;
	}
	data["Components"] = components;

	nlohmann::json children(nlohmann::detail::value_t::array);
	for (auto& child : myChildren)
	{
		children += child.Save();
	}
	data["Children"] = children;

	return data;
}
void EntityTemplate2::Load(const nlohmann::json& someData)
{
	if (someData.contains("PrefabPath"))
	{
		if (myPrefab)
		{
			delete myPrefab;
		}
		myPrefab = new EntityPrefab();
		myPrefab->Load(someData["PrefabPath"]);
	}
	else if (myPrefab)
	{
		delete myPrefab;
		myPrefab = nullptr;
	}

	myName = someData["Name"];

	myComponents.clear();

	auto& components = someData["Components"];

	if (myPrefab && components.size() != myPrefab->GetRawComponents().size())
	{
		std::cout << "[Error][EntityTemplate]: Error when trying to load prefab '" << myPrefab->GetPath() << "', Component list size mismatch with instance '" << myName << "'." << std::endl;
		InitBlankPrefabInstance();
	}
	else
	{
		size_t componentIndex = 0;
		for (auto& compData : components)
		{
			ComponentTemplate comp;
			comp.Type = compData["Type"];
			comp.Overrides = compData["Data"];
			if (myPrefab && comp.Type != myPrefab->GetRawComponents()[componentIndex++].Type)
			{
				std::cout << "[Error][EntityTemplate]: Error when trying to load prefab '" << myPrefab->GetPath() << "', Component list mismatch with instance '" << myName << "'." << std::endl;
				InitBlankPrefabInstance();
				break;
			}
			myComponents.push_back(comp);
		}
	}

	myChildren.clear();
	for (auto& child : someData["Children"])
	{
		EntityTemplate2 tChild;
		tChild.Load(child);
		myChildren.push_back(tChild);
	}
}
void EntityTemplate2::Reload()
{
	if (myPrefab)
	{
		myPrefab->Reload();
	}
}
void EntityTemplate2::AddComponent(const std::string& aType, const nlohmann::json& someOverrides)
{
	ComponentTemplate tComp;
	tComp.Type = aType;
	tComp.Overrides = someOverrides;
	TryAddComponent(tComp);
}
bool EntityTemplate2::TryAddComponent(const ComponentTemplate& aComponent)
{
	if (IsPrefabInstance())
	{
		return false;
	}
	else
	{
		myComponents.push_back(aComponent);
		return true;
	}
}
bool EntityTemplate2::TryEraseComponent(size_t aIndex)
{
	if (IsPrefabInstance())
	{
		return false;
	}
	else
	{
		myComponents.erase(myComponents.begin() + aIndex);
		return true;
	}
}
bool EntityTemplate2::TryAddChild(const EntityTemplate2& aEntity)
{
	if (IsPrefabInstance())
	{
		return false;
	}
	else
	{
		myChildren.push_back(aEntity);
		return true;
	}
}
void EntityTemplate2::InitBlankPrefabInstance()
{
	myComponents.clear();
	for (auto& comp : myPrefab->GetComponents())
	{
		ComponentTemplate tComp;
		tComp.Type = comp.Type;
		tComp.Overrides = {};
		myComponents.push_back(tComp);
	}
	myName = "Instance - " + myPrefab->GetTemplate().GetName();
}
//EntityTemplate::EntityTemplate()
//{
//	myName = "N/A";
//}
//
//EntityTemplate::Iterator EntityTemplate::begin()
//{
//	return myComponents.begin();
//}
//
//EntityTemplate::ConstIterator EntityTemplate::begin() const
//{
//	return myComponents.begin();
//}
//
//EntityTemplate::Iterator EntityTemplate::end()
//{
//	return myComponents.end();
//}
//
//EntityTemplate::ConstIterator EntityTemplate::end() const
//{
//	return myComponents.end();
//}
//
//EntityTemplate::Iterator EntityTemplate::erase(const Iterator& aPosition)
//{
//	assert(myPrefabPath.empty() && L"Cannot erase components of a prefab.");
//	return myComponents.erase(aPosition);
//}
//
//EntityTemplate::ConstIterator EntityTemplate::erase(const ConstIterator& aPosition)
//{
//	assert(myPrefabPath.empty() && L"Cannot erase components of a prefab.");
//	return myComponents.erase(aPosition);
//}
//
//void EntityTemplate::ClearOverrides()
//{
//	for (ComponentTemplate& comp : *this)
//	{
//		comp.Overrides = nlohmann::json(nlohmann::detail::value_t::null);
//	}
//}
//
//void EntityTemplate::SetName(const std::string& aName)
//{
//	myName = aName;
//}
//
//const std::string& EntityTemplate::GetName() const
//{
//	return myName;
//}
//
//bool EntityTemplate::SetPrefabPath(const std::string& aPath)
//{
//	if (!aPath.empty())
//	{
//		std::ifstream file(Tga::Settings::ResolveGameAssetPath(aPath));
//		if (file.is_open())
//		{
//			myPrefabPath = aPath;
//
//			EntityTemplate prefab;
//			prefab.Read(file);
//
//			assert(myComponents.size() == prefab.myComponents.size() && L"Could not set prefab. Amount of components differed.");
//			for (size_t ind = 0; ind < myComponents.size(); ind++)
//			{
//				ComponentTemplate& comp = myComponents[ind];
//				const ComponentTemplate& otherComp = prefab.myComponents[ind];
//
//				assert(comp.Type == otherComp.Type && L"Could set prefab. Component Types differed.");
//
//				comp.DefaultOverrides = JsonMerge(otherComp.DefaultOverrides, otherComp.Overrides, true);
//			}
//
//			return true;
//		}
//	}
//	else
//	{
//		myPrefabPath = aPath;
//		for (ComponentTemplate& comp : *this)
//		{
//			comp.DefaultOverrides = nlohmann::json(nlohmann::detail::value_t::null);
//		}
//		return true;
//	}
//	return false;
//}
//
//const std::string& EntityTemplate::GetPrefabPath() const
//{
//	return myPrefabPath;
//}
//
//bool EntityTemplate::IsPrefab() const
//{
//	return myPrefabPath.empty() == false;
//}
//
//EntityTemplate EntityTemplate::SumPrefabs() const
//{
//	if (myPrefabPath.empty())
//	{
//		EntityTemplate empty;
//
//		for (const ComponentTemplate& comp : *this)
//		{
//			empty.AddComponent(comp.Type);
//		}
//
//		return empty;
//	}
//	else
//	{
//		EntityTemplate sum;
//		std::ifstream file(Tga::Settings::ResolveGameAssetPath(myPrefabPath));
//		sum.Read(file);
//
//		sum = sum.Merge(sum.SumPrefabs());
//
//		return sum;
//	}
//}
//
//EntityTemplate EntityTemplate::Merge(const EntityTemplate& aEntity) const
//{
//	assert(myComponents.size() == aEntity.myComponents.size() && L"Could not merge. Amount of components differed.");
//	EntityTemplate merged;
//	for (size_t ind = 0; ind < myComponents.size(); ind++)
//	{
//		const ComponentTemplate& comp = myComponents[ind];
//		const ComponentTemplate& otherComp = aEntity.myComponents[ind];
//
//		assert(comp.Type == otherComp.Type && L"Could not merge. Component Types differed.");
//
//		merged.AddComponent(comp.Type, JsonMerge(otherComp.Overrides, comp.Overrides, true));
//	}
//	return merged;
//}
//
//void EntityTemplate::AddComponent(const std::string& aType, nlohmann::json someOverrides)
//{
//	assert(myPrefabPath.empty() && L"Cannot add components to a prefab.");
//
//	ComponentTemplate component = ComponentTemplate();
//	component.Type = aType;
//	component.Overrides = someOverrides;
//	component.DefaultOverrides = nlohmann::json(nlohmann::detail::value_t::null);
//	myComponents.push_back(component);
//}
//
//bool EntityTemplate::ContainsComponent(const std::string& aType) const
//{
//	for (const ComponentTemplate& component : myComponents)
//	{
//		if (component.Type == aType)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//const std::vector<ComponentTemplate>& EntityTemplate::GetComponents() const
//{
//	return myComponents;
//}
//
//const std::vector<EntityTemplate>& EntityTemplate::GetChildren() const
//{
//	return myChildren;
//}
//
//void EntityTemplate::Load(const nlohmann::json& someData)
//{
//	myComponents.clear();
//	myChildren.clear();
//	myPrefabPath = std::string();
//	myName = someData["Name"];
//
//	const nlohmann::json& components = someData["Components"];
//	for (auto& component : components)
//	{
//		AddComponent(component["Type"], component["Data"]);
//	}
//
//	if (someData.contains("IsPrefab") && someData["IsPrefab"])
//	{
//		SetPrefabPath(someData["PrefabPath"]);
//	}
//	else
//	{
//		SetPrefabPath(std::string());
//	}
//
//	const nlohmann::json& children = someData["Children"];
//	for (auto& child : children)
//	{
//		EntityTemplate tChild;
//		tChild.Load(child);
//		myChildren.push_back(tChild);
//	}
//}
//
//nlohmann::json EntityTemplate::Save() const
//{
//	nlohmann::json entityData;
//	entityData["Name"] = myName;
//	if (myPrefabPath.empty())
//	{
//		entityData["IsPrefab"] = false;
//	}
//	else
//	{
//		entityData["IsPrefab"] = true;
//		entityData["PrefabPath"] = myPrefabPath;
//	}
//
//	nlohmann::json components(nlohmann::detail::value_t::array);
//	for (const ComponentTemplate& compPtr : myComponents)
//	{
//		nlohmann::json compData;
//		compData.emplace("Type", compPtr.Type);
//		compData.emplace("Data", compPtr.Overrides);
//		components += compData;
//	};
//	entityData["Components"] = components;
//
//	nlohmann::json children(nlohmann::detail::value_t::array);
//	for (const EntityTemplate& child : myChildren)
//	{
//		children += child.Save();
//	}
//	entityData["Children"] = children;
//	return entityData;
//}
//
//void EntityTemplate::Read(std::istream& aStream)
//{
//	nlohmann::json entityData;
//	aStream >> entityData;
//
//	Load(entityData);
//}
//
//void EntityTemplate::Write(std::ostream& aStream) const
//{
//	nlohmann::json entityData = Save();
//	aStream << entityData;
//}

