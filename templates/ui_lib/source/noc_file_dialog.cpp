#include <ui_lib/helpers.h>

#define NOC_FILE_DIALOG_IMPLEMENTATION
#include "noc_file_dialog.h"


std::string ui::FileHelpers::GetOpenFileName()
{
    auto path = noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, nullptr, nullptr, "Some name");
    return std::string{path};
}

std::string ui::FileHelpers::GetSaveFileName()
{
    auto path = noc_file_dialog_open(NOC_FILE_DIALOG_SAVE | NOC_FILE_DIALOG_OVERWRITE_CONFIRMATION, nullptr, nullptr, "Some name");
    return std::string{path};
}
