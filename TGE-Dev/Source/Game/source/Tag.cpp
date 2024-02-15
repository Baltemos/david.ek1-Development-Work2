#include "Tag.h"
#include "Entity.h"

static std::unordered_map<std::string, std::multiset<Tag*>> locTags;

Tag* Tag::FindFirst(const std::string& aTag)
{
    auto entry = locTags.find(aTag);
    if (entry == locTags.end() || entry->second.empty()) return nullptr;
    return *entry->second.begin();
}

std::multiset<Tag*> Tag::FindAll(const std::string& aTag)
{
    auto entry = locTags.find(aTag);
    if (entry == locTags.end()) return {};
    else return entry->second;
}

bool Tag::HasTag(const std::shared_ptr<Entity>& aEntity, const std::string& aTag)
{
    std::vector<std::shared_ptr<Tag>> tags = aEntity->GetComponents<Tag>();
    for (auto& tagPtr : tags)
    {
        if (tagPtr->Compare(aTag))
        {
            return true;
        }
    }
    return false;
}

void Tag::SetTag(const std::string& aTag)
{
    RemoveEntry();
    myTag = aTag;
    AddEntry();
}

const std::string& Tag::GetTag() const
{
    return myTag;
}

bool Tag::Compare(const std::string& aTag) const
{
    return myTag == aTag;
}

bool Tag::Compare(const Tag& aTag) const
{
    return myTag == aTag.myTag;
}

void Tag::Read(const nlohmann::json& someData)
{
    myTag = someData["Tag"];

    AddEntry();
}

void Tag::OnDestroy()
{
    RemoveEntry();
}

void Tag::AddEntry()
{
    locTags[myTag].emplace(this);
}

void Tag::RemoveEntry()
{
    auto entry = locTags.find(myTag);
    if (entry == locTags.end())
    {
        throw std::exception(("Failed to remove tag. No tag with value '" + myTag + "'.").c_str());
    }
    if (entry->second.erase(this) == 0)
    {
        throw std::exception(("Failed to remove tag."));
    }
    if (entry->second.empty())
    {
        locTags.erase(entry);
    }
}
