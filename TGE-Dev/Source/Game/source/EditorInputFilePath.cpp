#include "EditorInputFilePath.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "FileDialog.h"
#include <tge/util/StringCast.h>
#include <tge/settings/settings.h>

bool EditorInputFilePath::OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& aEditorData)
{
	nlohmann::json value = JsonMerge(aDefault, aOverride, true);

	char buffer[_MAX_PATH];
	strncpy_s(buffer, value.get<std::string>().c_str(), sizeof(buffer));

	if (ImGui::Button("Open"))
	{
		std::wstring filter = string_cast<std::wstring>(aEditorData["Filter"].get<std::string>());
		size_t filterSize = filter.size() + 2;
		wchar_t* filterBuffer = new wchar_t[filterSize];
		ZeroMemory(filterBuffer, filterSize * sizeof(wchar_t));
		for (size_t ind = 0; ind < filter.size() && ind < filterSize; ind++)
		{
			wchar_t c = filter.at(ind);
			if (c == '|')
			{
				filterBuffer[ind] = '\0';
			}
			else
			{
				filterBuffer[ind] = c;
			}
		}


		FileDialogResult result = OpenFileDialog(std::filesystem::path(aEditorData["DefaultDirectory"].get<std::string>()), L"", filterBuffer);
		if (result.Succeeded)
		{
			std::filesystem::path dataPath("../Source/Game/data/");
			std::filesystem::path relativeToCurrentPath = std::filesystem::path(result.Path).lexically_relative(std::filesystem::current_path());
			std::filesystem::path relativeGame = relativeToCurrentPath.lexically_relative(dataPath);

			std::string p = string_cast<std::string>(relativeGame.c_str());
			aOverride = p;
			
			return true;
		}

		delete[] filterBuffer;
	}

	ImGui::SameLine();
	if (ImGui::InputText(aKey.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
	{
		aOverride = std::string(buffer);
		return true;
	}

	return false;
}
