add_library(stacktrace STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/crash_tracer.cpp)

target_include_directories(stacktrace SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(stacktrace PUBLIC absl::symbolize absl::failure_signal_handler)

set_target_properties(stacktrace PROPERTIES FOLDER "external")