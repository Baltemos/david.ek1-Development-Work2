#include "ScenePickerButton.h"
#include "FileDialog.h"
#include <tge/util/StringCast.h>
#include "GameWorld.h"

void ScenePickerButton::Click()
{
	std::filesystem::path def = std::filesystem::current_path() / Tga::Settings::ResolveGameAssetPath("");

	FileDialogResult result = OpenFileDialog(def, L".json", L"Json Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
	if (result.Succeeded)
	{
		std::string path = string_cast<std::string>(result.Path);
		GameWorld::GetInstance()->LoadSceneAbsolute(path);
	}
}
