contest_add_ui_library(ui_lib
    SOURCES
        include/ui_lib/ui_container.h
        include/ui_lib/ui_parameters_wrappers.h
        include/ui_lib/helpers.h
        source/imgui_visitor.cpp
    LIBS
        imgui
        plog
    )

cotire(ui_lib)
