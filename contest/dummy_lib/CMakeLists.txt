contest_add_library(dummy_lib
                                  SOURCES
                                    source/dummy.cpp
                                  LIBS 
                                    Boost::boost
                                    pplx
                                  TYPES static)
