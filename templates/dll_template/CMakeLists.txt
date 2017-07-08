contest_add_library(dll_template
                                  SOURCES
                                    source/dummy.cpp
                                  LIBS 
                                    Boost::boost
                                    plog
                                  TYPES shared)
