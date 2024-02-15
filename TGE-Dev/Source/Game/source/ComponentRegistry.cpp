#include "ComponentRegistry.h"
#include "Entity.h"

const ComponentRegistry::Entry& ComponentRegistry::GetEntry(const std::string& aName) const
{
	return myEntries.at(aName);
}

const std::set<std::string>& ComponentRegistry::GetRegistered() const
{
	return myRegistered;
}
