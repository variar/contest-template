
contest_add_ui(ui_exe_template
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        dll_template
        sfgui
        tclap
        plog
)

cotire(ui_exe_template)
