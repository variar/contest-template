
contest_add_exe(dummy_exe
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        dummy_lib_static
        sfgui
        tclap
        Boost::boost
        Boost::program_options
        tbb_static
        stlab
        lemon
        logger_static
)

cotire(dummy_exe)
