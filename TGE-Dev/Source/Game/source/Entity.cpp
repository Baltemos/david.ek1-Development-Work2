#include "Entity.h"

Entity::Entity()
{
	myIsDestroyed = false;
	myIsIndestructible = false;
}

void Entity::AddComponent(std::shared_ptr<Component> aComponent)
{
	myComponents.push_back(aComponent);
}

void Entity::Destroy()
{
	if (IsIndestructible() == false && !myIsDestroyed)
	{
		myIsDestroyed = true;
		OnDestroy();
	}
}

bool Entity::IsDestroyed() const
{
	return myIsDestroyed;
}

void Entity::Start()
{
	for (auto& compPtr : myComponents)
	{
		compPtr->Start();
	}
}

void Entity::Update(float aTimeDelta)
{
	for (auto& compPtr : myComponents)
	{
		if (compPtr->IsEnabled())
		{
			compPtr->Update(aTimeDelta);
		}
	}
}

void Entity::Render(Tga::GraphicsEngine& aGraphicsEngine)
{
	for (auto& compPtr : myComponents)
	{
		if (compPtr->IsEnabled())
		{
			compPtr->Render(aGraphicsEngine);
		}
	}
}

void Entity::OnDestroy()
{
	for (auto& compPtr : myComponents)
	{
		compPtr->OnDestroy();
	}
}

void Entity::SetIndestructible(bool aIndestructible)
{
	myIsIndestructible = aIndestructible;
}

bool Entity::IsIndestructible() const
{
	return myIsIndestructible;
}
