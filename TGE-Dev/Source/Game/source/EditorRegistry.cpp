#include "EditorRegistry.h"

std::shared_ptr<ValueEditor> EditorRegistry::Get(const std::string& aValueType)
{
    auto entry = myEditors.find(aValueType);
    if (entry == myEditors.end())
    {
        return nullptr;
    }
    return entry->second();
}

std::shared_ptr<ValueEditor> EditorRegistry::GetDefault()
{
    return std::make_shared<ValueEditor>(myDefaultEditor);
}
