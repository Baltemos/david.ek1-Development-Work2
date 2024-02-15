#include "EditorRegistry.h"

ValueEditor* EditorRegistry::Get(const std::string& aValueType)
{
    auto entry = myEditors.find(aValueType);
    if (entry == myEditors.end())
    {
        return nullptr;
    }
    return entry->second.get();
}

ValueEditor* EditorRegistry::GetDefault()
{
    return &myDefaultEditor;
}
