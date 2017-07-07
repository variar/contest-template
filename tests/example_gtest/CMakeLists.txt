contest_add_test(example_test_gtest
                               SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
                               LIBS gmock logger_static
                               TEST_DATA_DIR data)
