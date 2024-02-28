#include "EntityTemplate.h"
#include <fstream>
#include "JsonUtil.h"
#include <tge/settings/settings.h>
#include <sstream>

void EntityPrefab::Load(const std::string& aAssetPath)
{
	//try
	//{
	myPath = aAssetPath;
	std::string resolvedPath = Tga::Settings::ResolveGameAssetPath(aAssetPath);
	std::ifstream file(resolvedPath);
	nlohmann::json data;
	file >> data;
	myPrefab.Load(data);
	//}
	//catch (std::exception e)
	//{
	//	std::stringstream ss;
	//	ss << "Failed to load Prefab: " + aAssetPath + ". -> " << e.what() << std::endl;
	//	throw std::exception(ss.str().c_str());
	//}
}

void EntityPrefab::Reload()
{
	Load(myPath);
}

void EntityPrefab::Overwrite(const EntityTemplate& aEntity)
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

std::vector<EntityTemplate> EntityPrefab::GetChildren() const
{
	return myPrefab.GetChildren();
}

const EntityTemplate& EntityPrefab::GetTemplate() const
{
	return myPrefab;
}

const std::string& EntityPrefab::GetPath() const
{
	return myPath;
}

bool EntityPrefab::Empty() const
{
	return myPath.empty();
}

EntityTemplate::EntityTemplate()
{
	myPrefab = nullptr;
}
EntityTemplate::EntityTemplate(const std::string& aPrefabPath)
{
	myPrefab = new EntityPrefab();
	myPrefab->Load(aPrefabPath);
	InitBlankPrefabInstance();
}
EntityTemplate::~EntityTemplate()
{
	if (myPrefab)
	{
		delete myPrefab;
	}
}
EntityTemplate& EntityTemplate::operator=(const EntityTemplate& aEntity)
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
EntityTemplate::EntityTemplate(const EntityTemplate& aEntity)
{
	*this = aEntity;
}
void EntityTemplate::SetName(const std::string& aName)
{
	myName = aName;
}
const std::string& EntityTemplate::GetName() const
{
	return myName;
}
ComponentTemplate EntityTemplate::GetComponent(std::string aType) const
{
	for (const ComponentTemplate& component : GetComponents())
	{
		if (component.Type == aType)
		{
			return component;
		}
	}

	return {};
}
std::vector<ComponentTemplate> EntityTemplate::GetComponents() const
{
	std::vector<ComponentTemplate> components = myComponents;
	if (myPrefab)
	{
		std::vector<ComponentTemplate> prefabComponents = myPrefab->GetComponents();
		for (size_t ind = 0; ind < components.size(); ind++)
		{
			components[ind].Defaults = JsonMerge(prefabComponents[ind].Defaults, prefabComponents[ind].Overrides);
		}
	}
	return components;
}
std::vector<ComponentTemplate>& EntityTemplate::GetRawComponents()
{
	return myComponents;
}
std::vector<EntityTemplate> EntityTemplate::GetChildren() const
{
	std::vector<EntityTemplate> children = myChildren;
	if (IsPrefabInstance())
	{
		std::vector<EntityTemplate> prefabChildren = myPrefab->GetChildren();
		children.insert(children.end(), prefabChildren.begin(), prefabChildren.end());
	}
	return children;
}
std::vector<EntityTemplate>& EntityTemplate::GetRawChildren()
{
	return myChildren;
}
const std::vector<EntityTemplate>& EntityTemplate::GetRawChildren() const
{
	return myChildren;
}
std::vector<EntityTemplate> EntityTemplate::GetPrefabChildren() const
{
	if (myPrefab)
	{
		return myPrefab->GetChildren();
	}
	else return {};
}
const EntityPrefab* EntityTemplate::GetPrefab() const
{
	return myPrefab;
}
bool EntityTemplate::ContainsComponent(const std::string& aType) const
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
void EntityTemplate::ClearOverrides()
{
	for (auto& comp : myComponents)
	{
		comp.Overrides = nlohmann::json(nlohmann::detail::value_t::object);
	}
}
bool EntityTemplate::IsPrefabInstance() const
{
	return myPrefab != nullptr;
}
EntityTemplate EntityTemplate::GetIndependent() const
{
	EntityTemplate tEntity;
	tEntity.myChildren = GetChildren();

	std::vector<ComponentTemplate> components = GetComponents();
	for (ComponentTemplate comp : components)
	{
		comp.Overrides = JsonMerge(comp.Defaults, comp.Overrides);
		comp.Defaults = nlohmann::json(nlohmann::detail::value_t::object);
		tEntity.TryAddComponent(comp);
	}
	return tEntity;
}
void EntityTemplate::OverwritePrefab()
{
	EntityTemplate overwritten = myPrefab->GetTemplate();

	for (size_t ind = 0; ind < overwritten.myComponents.size(); ind++)
	{
		overwritten.myComponents[ind].Overrides = JsonMerge(overwritten.myComponents[ind].Overrides, myComponents[ind].Overrides, true);
	}

	myPrefab->Overwrite(overwritten);
	ClearOverrides();
}
nlohmann::json EntityTemplate::Save() const
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
void EntityTemplate::Load(const nlohmann::json& someData)
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

	if (myPrefab && components.is_null())
	{
		InitBlankPrefabInstance();
	}
	else if (myPrefab && components.size() != myPrefab->GetRawComponents().size())
	{
		std::cout << "[Warning][EntityTemplate]: Warning when trying to load prefab '" << myPrefab->GetPath() << "', Component list size mismatch with instance '" << myName << "'." << std::endl;

		nlohmann::json transform = FetchComponent("Transform", components);
		nlohmann::json transformData = nlohmann::json(nlohmann::detail::value_t::object);
		if (!transform.is_null())
		{
			transformData = transform["Data"];
		}
		InitBlankPrefabInstance(transformData);
	}
	else
	{
		size_t componentIndex = 0;
		for (auto& compData : components)
		{
			ComponentTemplate comp;
			comp.Type = compData["Type"];
			comp.Overrides = compData["Data"];
			//if (comp.Overrides.is_null())
			//{
			//	std::cout << "[Warning][" << __FILE__ << "]: Component overrides was null, this should allways be an object. Replacing with object." << std::endl;
			//	comp.Overrides = nlohmann::json(nlohmann::detail::value_t::object);
			//}

			if (myPrefab && comp.Type != myPrefab->GetRawComponents()[componentIndex++].Type)
			{
				std::cout << "[Warning][EntityTemplate]: Warning when trying to load prefab '" << myPrefab->GetPath() << "', Component list mismatch with instance '" << myName << "'." << std::endl;

				nlohmann::json transform = FetchComponent("Transform", components);
				nlohmann::json transformData = nlohmann::json(nlohmann::detail::value_t::object);
				if (!transform.is_null())
				{
					transformData = transform["Data"];
				}
				InitBlankPrefabInstance(transformData);
				break;
			}
			myComponents.push_back(comp);
		}
	}

	myChildren.clear();
	for (auto& child : someData["Children"])
	{
		EntityTemplate tChild;
		tChild.Load(child);
		myChildren.push_back(tChild);
	}
}
void EntityTemplate::Reload()
{
	if (myPrefab)
	{
		myPrefab->Reload();
	}
}
void EntityTemplate::AddComponent(const std::string& aType, const nlohmann::json& someOverrides)
{
	ComponentTemplate tComp;
	tComp.Type = aType;
	tComp.Overrides = someOverrides;
	TryAddComponent(tComp);
}
bool EntityTemplate::TryAddComponent(const ComponentTemplate& aComponent)
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
bool EntityTemplate::TryEraseComponent(size_t aIndex)
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
bool EntityTemplate::TryAddChild(const EntityTemplate& aEntity)
{
	//if (IsPrefabInstance())
	//{
	//	return false;
	//}
	//else
	//{
	myChildren.push_back(aEntity);
	return true;
	//}
}
nlohmann::json EntityTemplate::FetchComponent(const std::string& aType, const nlohmann::json& aComponents)
{
	for (auto& comp : aComponents)
	{
		if (comp["Type"] == aType)
		{
			return comp;
		}
	}
	return nlohmann::json(nlohmann::detail::value_t::null);
}
void EntityTemplate::InitBlankPrefabInstance(const nlohmann::json& aOptTransformOverride)
{
	myComponents.clear();
	for (auto& comp : myPrefab->GetComponents())
	{
		ComponentTemplate tComp;
		tComp.Type = comp.Type;
		if (tComp.Type == "Transform")
		{
			tComp.Overrides = aOptTransformOverride;
		}
		else
		{
			tComp.Overrides = nlohmann::json(nlohmann::detail::value_t::object);
		}
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

