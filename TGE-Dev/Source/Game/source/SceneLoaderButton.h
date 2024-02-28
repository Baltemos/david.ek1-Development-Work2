#pragma once
#include "Button.h"

class SceneLoaderButton : public Button
{
protected:
	// Inherited via Button
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Click() override;

private:
	std::string mySceneAssetPath;
};