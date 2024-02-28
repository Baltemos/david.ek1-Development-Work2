#pragma once
#include "Component.h"
#include <CommonUtilities/InputHandler.h>

namespace cu = CommonUtilities;

class UIElement;

class UIManager : public Component
{
public:
	cu::InputHandler* GetInputHandler() const;

	void Register(std::shared_ptr<UIElement> aUIElement);
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Update(float aTimeDelta) override;
private:
	cu::InputHandler* myInputHandler;

	std::vector<std::shared_ptr<UIElement>> myUIElements;
	std::vector<std::shared_ptr<UIElement>> myClickedUIElements;
};