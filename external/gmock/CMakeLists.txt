add_library(gmock INTERFACE)
target_include_directories(gmock INTERFACE "include")
target_sources(gmock INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/source/gmock_gtest.cpp")
