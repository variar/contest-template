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

if (WIN32)
target_link_libraries(ui_lib PUBLIC comdlg32 ole32)
endif()

#cotire(ui_lib)
