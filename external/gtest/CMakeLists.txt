add_library(gtest STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gmock_gtest.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
)
target_include_directories(gtest SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
