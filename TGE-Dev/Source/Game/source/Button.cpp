#include "Button.h"

void Button::Read(const nlohmann::json& someData)
{
	UIElement::Read(someData);

	OnClicked.Subscribe(this, &Button::onClickedDown);
	OnClickedReleased.Subscribe(this, &Button::onClickedReleased);
}

void Button::onClickedDown(std::shared_ptr<UIElement>)
{

}

void Button::onClickedReleased(std::shared_ptr<UIElement>)
{
	if (IsHovered())
	{
		Click();
	}
}
