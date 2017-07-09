contest_add_test(example_test_catch
                               SOURCES 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/source/some_test.cpp
                               LIBS catch Boost::filesystem
                               TEST_DATA_DIR data)
