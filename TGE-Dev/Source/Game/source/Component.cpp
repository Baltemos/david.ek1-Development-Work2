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

void Component::AssignEntity(std::shared_ptr<Entity> aEntity)
{
    myEntity = aEntity;
}
