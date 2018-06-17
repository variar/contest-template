#include <ui_lib/helpers.h>
#include "tinyfiledialogs.h"

std::string ui::FileHelpers::GetOpenFileName()
{
    auto path = tinyfd_openFileDialog("Title", nullptr, 0, nullptr, nullptr, 0);
    if (path == nullptr)
    {
        return {};
    }
    return std::string{path};
}

std::string ui::FileHelpers::GetSaveFileName()
{
    auto path = tinyfd_saveFileDialog("Title", nullptr, 0, nullptr, nullptr);
    if (path == nullptr)
    {
        return {};
    }
    return std::string{path};
}