#include "UIManager.h"
#include "UIElement.h"
#include "GameWorld.h"

cu::InputHandler* UIManager::GetInputHandler() const
{
	return myInputHandler;
}

void UIManager::Register(std::shared_ptr<UIElement> aUIElement)
{
	myUIElements.push_back(aUIElement);
}

void UIManager::Read(const nlohmann::json& someData)
{
	someData;
	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();
}

void UIManager::Update(float aTimeDelta)
{
	aTimeDelta;

	myUIElements.erase(
		std::remove_if(
			myUIElements.begin(),
			myUIElements.end(),
			[](std::shared_ptr<UIElement>& element) {
				return element->IsDestroyed();
			}
		),
		myUIElements.end()
	);

	if (myInputHandler->IsKeyDown(static_cast<int>(cu::Keys::MOUSELBUTTON)))
	{
		for (std::shared_ptr<UIElement>& element : myUIElements)
		{
			if (element->IsHovered())
			{
				myClickedUIElements.push_back(element);
				element->OnClicked(element);
			}
		}
	}
	if (myInputHandler->IsKeyUp(static_cast<int>(cu::Keys::MOUSELBUTTON)))
	{
		for (std::shared_ptr<UIElement>& element : myClickedUIElements)
		{
			element->OnClickedReleased(element);
		}
		myClickedUIElements.clear();
	}
}
