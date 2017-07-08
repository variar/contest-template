add_library(gmock INTERFACE)
target_include_directories(gmock INTERFACE "include")

add_library(gmock_gtest INTERFACE)
target_link_libraries(gmock_gtest INTERFACE gmock)
target_sources(gmock_gtest INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/source/gmock_gtest.cpp")
