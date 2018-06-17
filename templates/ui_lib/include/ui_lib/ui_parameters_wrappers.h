#pragma once

#include <imgui/imgui.h>

#include <absl/types/variant.h>
#include <functional>
#include <string>
#include <vector>

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
    using ValueT = absl::variant<std::string, bool, int, float, RadioOptions, ComboOptions, Progress>;

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
        return absl::get<T>(param->value);
    }

    return {};
}

class ImGuiParamsVisitor
{
    public:
        ImGuiParamsVisitor(const std::string& paramName, bool isReadOnly);

        ImGuiParamsVisitor(const ImGuiParamsVisitor&) = delete;
        ImGuiParamsVisitor(ImGuiParamsVisitor&&) = delete;

        ImGuiParamsVisitor& operator=(const ImGuiParamsVisitor&) = delete;
        ImGuiParamsVisitor& operator=(ImGuiParamsVisitor&&) = delete;


        void operator()(std::string& param);
        void operator()(bool& param);
        void operator()(int& param);
        void operator()(float& param);

        void operator()(RadioOptions& param);
        void operator()(ComboOptions& param);

        void operator()(Progress& param);

    private:
        std::string m_paramName;
        std::string m_label;

        ImGuiInputTextFlags m_flags;
};

}