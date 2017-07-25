add_library(catch STATIC "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp")
target_include_directories(catch SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(catch PROPERTIES FOLDER "external")
