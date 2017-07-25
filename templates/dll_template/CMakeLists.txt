contest_add_library(dll_template
                                  SOURCES
                                    source/dummy.cpp
                                  LIBS 
                                    Boost::boost
                                    plog
                                    stacktrace
                                  TYPES shared)
