contest_add_ui_library(ui_lib
    SOURCES
        include/ui_lib/ui_container.h
        include/ui_lib/ui_parameters_wrappers.h
        include/ui_lib/helpers.h
        source/imgui_visitor.cpp
        source/tinyfiledialogs.c
        source/helpers.cpp
    LIBS
        imgui
        plog
        absl::variant
        absl::optional
        gsl
    )

#cotire(ui_lib)
