 # ex: contest_add_test(test_exe_name
#                      SOURCES sprite.cpp image.cpp ...
#                      DEPENDS catch dummy_lib
#                      TEST_DATA_DIR data)
 macro (contest_add_test target)
    #  parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;TEST_DATA_DIR" ${ARGN})
    
    add_executable(${target} ${THIS_SOURCES})
    
    if(THIS_DEPENDS)
        target_link_libraries(${target} PRIVATE ${THIS_DEPENDS})
    endif()
    
    if(THIS_TEST_DATA_DIR)
      add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${THIS_TEST_DATA_DIR} 
                $<TARGET_FILE_DIR:${target}>/test_data/${target}/${TEST_DATA_DIR})
    endif()
endmacro()

add_subdirectory(example_gtest)
add_subdirectory(example_catch)
