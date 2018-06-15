
contest_add_exe(exe_template
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        lib_template
        cli11
        Boost::boost
        stlab
        lemon
        plog
        gsl
        mpir
        absl::strings
        absl::symbolize
        absl::failure_signal_handler
)

cotire(exe_template)
