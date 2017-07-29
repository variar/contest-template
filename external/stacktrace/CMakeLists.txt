if(WIN32)
    set(STACKTRACE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/windbg.cpp)
else()
    set(STACKTRACE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/basic.cpp)
endif()

add_library(stacktrace STATIC ${STACKTRACE_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/src/crash_tracer.cpp)
target_include_directories(stacktrace SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(stacktrace PUBLIC Boost_filesystem)
target_compile_definitions(stacktrace INTERFACE BOOST_STACKTRACE_LINK)

if(WIN32)
    target_compile_definitions(stacktrace INTERFACE BOOST_STACKTRACE_USE_WINDBG=1)
    target_link_libraries(stacktrace PUBLIC dbgeng ole32)
else()
    target_link_libraries(stacktrace PUBLIC dl)
endif()

set_target_properties(stacktrace PROPERTIES FOLDER "external")