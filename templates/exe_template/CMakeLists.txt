
contest_add_exe(exe_template
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    LIBS
        lib_template
        tclap
        Boost::boost
        stlab
        lemon
        plog
        gsl
        mpir
)

cotire(exe_template)
