add_executable(example_test
	source/main.cpp
)

target_link_libraries(example_test
	gmock
)

set(TEST_DATA_DIR data)

add_custom_command(
                TARGET example_test PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DATA_DIR} 
                $<TARGET_FILE_DIR:example_test>/${TEST_DATA_DIR})
