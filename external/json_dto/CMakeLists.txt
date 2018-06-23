add_library(json_dto INTERFACE)
target_include_directories(json_dto SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(json_dto INTERFACE rapidjson)
