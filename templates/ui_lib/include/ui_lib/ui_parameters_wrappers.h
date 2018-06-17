#pragma once

#include <imgui/imgui.h>

#include <variant>

namespace ui
{


struct RadioTag {};
struct ComboTag {};

template<typename UiTag>
struct Options
{
    Options() : current {}
    {}

    std::vector<std::string> options;
    int current;
};

using ComboOptions = Options<ComboTag>;
using RadioOptions = Options<RadioTag>;

struct Progress
{
    Progress(float v = 0.f) : value {v}
    {}

    float value;
};

struct WorldParameter
{
    using ValueT = std::variant<std::string, bool, int, float, RadioOptions, ComboOptions, Progress>;

    WorldParameter(std::string name, ValueT val, bool readOnly = false)
        : name {std::move(name)}
        , value{std::move(val)}
        , isReadOnly {readOnly}
    {}

    std::string name;
    ValueT value;
    bool isReadOnly;
};

using WorldParameters = std::vector<WorldParameter>;

struct WorldAction
{
    std::string name;
    std::function<void()> execute;
};

using WorldActions = std::vector<WorldAction>;

template<typename T>
T GetParamByName(const WorldParameters& params, const std::string& name)
{
    auto param = std::find_if(params.begin(), params.end(), [&name](const auto& p)
    {
        return name == p.name;
    });

    if (param != params.end()) 
    {
        return std::get<T>(param->value);
    }

    return {};
}

class ImGuiParamsVisitor
{
    public:
        ImGuiParamsVisitor(const std::string& paramName, bool isReadOnly)
        : m_paramName{paramName}
        , m_label{ "##" + paramName }
        , m_flags{isReadOnly ? ImGuiInputTextFlags_ReadOnly :  ImGuiInputTextFlags_None} 
        {
            ImGui::Text("%s", m_paramName.c_str());
        }

        ImGuiParamsVisitor(const ImGuiParamsVisitor&) = delete;
        ImGuiParamsVisitor(ImGuiParamsVisitor&&) = delete;

        ImGuiParamsVisitor& operator=(const ImGuiParamsVisitor&) = delete;
        ImGuiParamsVisitor& operator=(ImGuiParamsVisitor&&) = delete;


        void operator()(std::string& param)
        {
            auto buffer = std::vector<char>();
            buffer.resize(param.size() + 1024);
            std::strcpy(buffer.data(), param.c_str());

            if (ImGui::InputText(m_label.c_str(), buffer.data(), buffer.size(), m_flags))
            {
                param = std::string(buffer.data());
            }
        }

        void operator()(bool& param)
        {
            ImGui::Checkbox(m_label.c_str(), &param);
        }

        void operator()(int& param)
        {
            ImGui::InputInt(m_label.c_str(), &param, 1, 100, m_flags);
        }

        void operator()(float& param)
        {
            ImGui::InputFloat(m_label.c_str(), &param, 1.f, 10.f, "%.6f", m_flags);
        }

        void operator()(RadioOptions& param)
        {
            for (size_t i = 0; i < param.options.size(); ++i)
            {
                ImGui::RadioButton(param.options[i].c_str(), &param.current, i);
            }
        }

        void operator()(ComboOptions& param)
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

        void operator()(Progress& param)
        {
            ImGui::ProgressBar(param.value);
        }


    private:
        std::string m_paramName;
        std::string m_label;

        ImGuiInputTextFlags m_flags;
};

}