#include "SceneLoaderButton.h"
#include "GameWorld.h"

void SceneLoaderButton::Read(const nlohmann::json& someData)
{
	Button::Read(someData);

	mySceneAssetPath = someData["Scene"];
}

void SceneLoaderButton::Click()
{
	GameWorld::GetInstance()->LoadScene(mySceneAssetPath);
}
