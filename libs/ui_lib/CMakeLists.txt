contest_add_ui_library(ui_lib
    SOURCES
        source/imgui_visitor.cpp
        source/tinyfiledialogs.c
        source/helpers.cpp
        source/ui_frame.cpp
        include/ui_lib/world.h
        include/ui_lib/ui_frame.h
        include/ui_lib/world_parameters.h
        include/ui_lib/helpers.h
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

set_target_properties(baseapp PROPERTIES FOLDER "libs")

cotire(ui_lib)
