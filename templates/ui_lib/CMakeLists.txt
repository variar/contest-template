contest_add_ui_library(ui_lib
    SOURCES
        include/ui_lib/ui_container.h
    LIBS
        imgui
        plog
    )

cotire(ui_lib)
