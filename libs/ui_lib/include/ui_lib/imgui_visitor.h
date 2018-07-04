#pragma once

#include <ui_lib/world_parameters.h>

namespace ui
{
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
        bool m_isReadOnly;
};
}