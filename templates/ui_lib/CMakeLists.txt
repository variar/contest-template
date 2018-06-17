contest_add_ui_library(ui_lib
    SOURCES
        include/ui_lib/ui_container.h
        include/ui_lib/ui_parameters_wrappers.h
        include/ui_lib/helpers.h
        source/imgui_visitor.cpp
        source/noc_file_dialog.cpp
    LIBS
        imgui
        plog
        absl::variant
        absl::optional
        gsl
    )

if (UNIX)
        if(APPLE)
            target_compile_definitions(ui_lib PUBLIC -DNOC_FILE_DIALOG_OSX)
        else()
            target_compile_definitions(ui_lib PUBLIC -DNOC_FILE_DIALOG_GTK)

            FIND_PACKAGE(PkgConfig REQUIRED)
            PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

            # Setup CMake to use GTK+, tell the compiler where to look for headers
            # and to the linker where to look for libraries
            target_include_directories(ui_lib PRIVATE ${GTK3_INCLUDE_DIRS})
            target_link_libraries(ui_lib PUBLIC ${GTK3_LIBRARIES})

            # Add other flags to the compiler
            target_compile_options(ui_lib PUBLIC ${GTK3_CFLAGS_OTHER})
        endif()
else()
    target_compile_definitions(ui_lib PUBLIC -DNOC_FILE_DIALOG_WIN32)
endif()

#cotire(ui_lib)
