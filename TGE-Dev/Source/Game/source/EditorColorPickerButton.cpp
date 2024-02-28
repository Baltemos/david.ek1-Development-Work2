#include "EditorColorPickerButton.h"
#include "JsonUtil.h"
#include <imgui/imgui.h>

bool EditorColorPickerButton::OnEditValue(nlohmann::json& aOverride, EditorRegistry&)
{
    nlohmann::json current = GetValue(GetDefaultValue(), aOverride);

    float value[4];
    for (size_t index = 0; index < 4; index++)
    {
        value[index] = current[index];
    }

    if (ImGui::ColorEdit4(GetKey().c_str(), value))
    {
        for (size_t index = 0; index < 4; index++)
        {
            aOverride[index] = value[index];
        }
        return true;
    }
    else return false;
}
