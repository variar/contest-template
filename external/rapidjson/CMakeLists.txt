add_library(rapidjson INTERFACE)
target_include_directories(rapidjson SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
