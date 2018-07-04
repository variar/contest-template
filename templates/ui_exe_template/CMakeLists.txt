contest_add_ui_exe(ui_exe_template
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        baseapp
        dll_template
        ui_lib
        imgui
        cli11
        plog
        absl::symbolize
        absl::failure_signal_handler
)

cotire(ui_exe_template)
