contest_add_library(lib_template
                                  SOURCES
                                    source/dummy.cpp
                                  LIBS 
                                    Boost::boost
                                    plog
                                    mpir
                                  TYPES static)
