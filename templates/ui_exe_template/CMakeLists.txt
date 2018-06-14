contest_add_ui_exe(ui_exe_template
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        dll_template
        imgui
        cli11
        plog
)

cotire(ui_exe_template)
