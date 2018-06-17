#include <ui_lib/ui_parameters_wrappers.h>

namespace ui
{

ImGuiParamsVisitor::ImGuiParamsVisitor(const std::string& paramName, bool isReadOnly)
: m_paramName{paramName}
, m_label{ "##" + paramName }
, m_flags{isReadOnly ? ImGuiInputTextFlags_ReadOnly :  ImGuiInputTextFlags_None} 
{
    ImGui::Text("%s", m_paramName.c_str());
}

void ImGuiParamsVisitor::operator()(std::string& param)
{
    auto buffer = std::vector<char>();
    buffer.resize(param.size() + 1024);
    std::strcpy(buffer.data(), param.c_str());

    if (ImGui::InputText(m_label.c_str(), buffer.data(), buffer.size(), m_flags))
    {
        param = std::string(buffer.data());
    }
}

void ImGuiParamsVisitor::operator()(bool& param)
{
    ImGui::Checkbox(m_label.c_str(), &param);
}

void ImGuiParamsVisitor::operator()(int& param)
{
    ImGui::InputInt(m_label.c_str(), &param, 1, 100, m_flags);
}

void ImGuiParamsVisitor::operator()(float& param)
{
    ImGui::InputFloat(m_label.c_str(), &param, 1.f, 10.f, "%.6f", m_flags);
}

void ImGuiParamsVisitor::operator()(RadioOptions& param)
{
    for (size_t i = 0; i < param.options.size(); ++i)
    {
        ImGui::RadioButton(param.options[i].c_str(), &param.current, i);
    }
}

void ImGuiParamsVisitor::operator()(ComboOptions& param)
{
    if(ImGui::BeginCombo(m_label.c_str(), param.options[param.current].c_str()))
    {
        for (int i = 0; static_cast<size_t>(i) < param.options.size(); ++i)
        {
            auto isSelected = (param.current == i);
            if (ImGui::Selectable(param.options[i].c_str(), isSelected))
            {
                param.current = i;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void ImGuiParamsVisitor::operator()(Progress& param)
{
    ImGui::ProgressBar(param.value);
}

}