
contest_add_exe(dummy_exe
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        dummy_lib
        sfgui
        tclap
        Boost::boost
        Boost::program_options
        tbb_static
        stlab
        lemon
)

cotire(dummy_exe)
