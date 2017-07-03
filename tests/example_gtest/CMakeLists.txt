contest_add_test(example_test_gtest
                               SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
                               DEPENDS gmock
                               TEST_DATA_DIR data)
