#include "UIElement.h"
#include "Entity.h"

UIElement::UIElement()
{
	myIsHovered = false;
}

std::shared_ptr<Transform> UIElement::GetTransform() const
{
	return myTransform.lock();
}

std::shared_ptr<UIManager> UIElement::GetUIManager() const
{
	return myUIManager.lock();
}

cu::AABB2D<float> UIElement::GetBounds() const
{
	return myWorldBounds;
}

bool UIElement::IsHovered() const
{
	return myIsHovered;
}

void UIElement::UpdateBounds()
{
	std::shared_ptr<Transform> transform = GetTransform();
	cu::Vector2<float> worldPos = transform->GetWorldPosition().ConvertToVector2XY();
	myWorldBounds.Min = worldPos + myLocalBounds.Min;
	myWorldBounds.Max = worldPos + myLocalBounds.Max;
}

void UIElement::Read(const nlohmann::json& someData)
{
	myTransform = GetEntity()->GetComponent<Transform>();

	{
		auto& bounds = someData["Bounds"];
		{
			auto& min = bounds["Min"];
			myLocalBounds.Min.x = min[0];
			myLocalBounds.Min.y = min[1];

			auto& max = bounds["Max"];
			myLocalBounds.Max.x = max[0];
			myLocalBounds.Max.y = max[1];
		}
	}

	UpdateBounds();
}

void UIElement::Start()
{
	myUIManager = FindUIManager();
	GetUIManager()->Register(std::dynamic_pointer_cast<UIElement>(GetSharedPtr()));
}

void UIElement::Update(float aTimeDelta)
{
	aTimeDelta;

	UpdateBounds();

	POINT mousePos = GetUIManager()->GetInputHandler()->GetMouseClientPosition();
	cu::Vector2<float> mouseWorldPos = GetTransform()->ScreenToWorld({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) }).ConvertToVector2XY();

	myIsHovered = GetBounds().IsInside(mouseWorldPos);
}

std::weak_ptr<UIManager> UIElement::FindUIManager()
{
	std::shared_ptr<Transform> transform = myTransform.lock();
	std::shared_ptr<UIManager> manager = nullptr;
	while (transform)
	{
		manager = transform->GetEntity()->GetComponent<UIManager>();
		if (manager)
		{
			break;
		}
		transform = transform->GetParent();
	}
	return manager;
}
