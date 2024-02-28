#pragma once
#include "UIElement.h"

class Button : public UIElement
{
public:
protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Click() = 0;
private:
	void onClickedDown(std::shared_ptr<UIElement>);
	void onClickedReleased(std::shared_ptr<UIElement>);
};