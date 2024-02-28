#include "Component.h"
#include "Entity.h"

Component::Component()
{
    myIsEnabled = true;
}

std::shared_ptr<Entity> Component::GetEntity() const
{
    return myEntity.lock();
}

bool Component::IsDestroyed() const
{
    return myEntity.expired() || myEntity.lock()->IsDestroyed();
}

void Component::SetEnabled(bool aEnabled)
{
    myIsEnabled = aEnabled;
}

bool Component::IsEnabled() const
{
    return myIsEnabled;
}

std::shared_ptr<Component> Component::GetSharedPtr()
{
    return mySharedPtr.lock();
}

std::shared_ptr<const Component> Component::GetSharedPtr() const
{
    return mySharedPtr.lock();
}

void Component::AssignEntity(std::shared_ptr<Entity> aEntity)
{
    myEntity = aEntity;
}
