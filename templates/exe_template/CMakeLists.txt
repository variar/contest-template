
contest_add_exe(exe_template
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        baseapp
        lib_template
        cli11
        Boost::boost
        stlab
        plog
        gsl
        mpirxx
        absl::strings
        absl::optional
        absl::variant
)

target_compile_definitions(exe_template PRIVATE PLOG_DISABLE_LOGGING=1)

cotire(exe_template)
