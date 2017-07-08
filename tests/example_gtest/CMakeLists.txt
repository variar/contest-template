contest_add_test(example_test_gtest
                               SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/some_test.cpp
                               LIBS gtest_main_lib Boost::filesystem
                               TEST_DATA_DIR data)
