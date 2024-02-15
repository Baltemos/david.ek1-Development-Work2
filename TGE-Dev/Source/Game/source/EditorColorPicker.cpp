#include "EditorColorPicker.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>

bool EditorColorPicker::OnEditValue(const std::string& aKey, const nlohmann::json& aDefault, nlohmann::json& aOverride, const nlohmann::json& /*aEditorData*/)
{
    nlohmann::json current = JsonMerge(aDefault, aOverride, true);
    
    float value[4];
    for (size_t index = 0; index < 4; index++)
    {
        value[index] = current[index];
    }

    if (ImGui::ColorPicker4(aKey.c_str(), value))
    {
        for (size_t index = 0; index < 4; index++)
        {
            aOverride[index] = value[index];
        }
        return true;
    }
    else return false;
}
