include(GenerateExportHeader)

macro(contest_wall target)
    #see https://lefticus.gitbooks.io/cpp-best-practices/content/02-Use_the_Tools_Available.html
    if(UNIX)
        target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -pedantic)
    elseif(WIN32)
        target_compile_definitions(${target} PRIVATE  /W4 /W44640 /w14265 /we4289 /w14296 /w14640 /w14905 /w14906 /w14928)
    endif()
endmacro()

macro(contest_stdafx_cpp)
     if(WIN32)
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.cpp)
            message("Found stdafx in ${CMAKE_CURRENT_SOURCE_DIR}/source")
            list(INSERT THIS_SOURCES 0 ${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.cpp)
        endif()
    endif()
endmacro()

macro(contest_stdafx_h target)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.h)
        message("Found stdafx.h in ${CMAKE_CURRENT_SOURCE_DIR}/source")
        set_target_properties(${target} PROPERTIES
            COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.h"
        )
    endif()
endmacro()

# ex: contest_add_library(test_exe_name
#                      SOURCES sprite.cpp image.cpp ...
#                      INCLUDES path1 path2 ...
#                      LIBS dummy_lib ...)
macro(contest_add_basic_library target type output)
    cmake_parse_arguments(THIS "" "" "TYPE;SOURCES;INCLUDES;LIBS" ${ARGN})
    
    contest_stdafx_cpp()
    
    add_library(${target} ${type}	${THIS_SOURCES})
    generate_export_header(${target} BASE_NAME ${output})
       
    if(THIS_INCLUDES)
        target_include_directories(${target} PUBLIC ${THIS_INCLUDES})
    endif()
    
    if(THIS_LIBS)
        target_link_libraries(${target} PUBLIC ${THIS_LIBS})
    endif()
    
    contest_wall(${target})
    contest_stdafx_h(${target})

endmacro()

# ex: contest_add_library(test_exe_name
#                      SOURCES sprite.cpp image.cpp ...
#                      LIBS dummy_lib
#                      TYPES shared;static)
macro (contest_add_library target)
    cmake_parse_arguments(THIS "" "" "SOURCES;LIBS;TYPES" ${ARGN})
  
    set(THIS_LIB_TYPES ";${THIS_TYPES};")
  
    set(THIS_INCLUDE_PATHS 
            ${CMAKE_CURRENT_BINARY_DIR}
            "${CMAKE_CURRENT_SOURCE_DIR}/include")
  
    if("${THIS_LIB_TYPES}" MATCHES ";static;|;;")
        set(STATIC_TARGET ${target}_static)
        contest_add_basic_library(
            ${STATIC_TARGET} STATIC
            ${target}
            SOURCES ${THIS_SOURCES}
            INCLUDES ${THIS_INCLUDE_PATHS}
            LIBS ${THIS_LIBS}
        )
                                                                
        string(TOUPPER ${STATIC_TARGET} UPPER_TARGET)
        target_compile_definitions(${STATIC_TARGET} PUBLIC -D${UPPER_TARGET}_DEFINE)
    endif()
    
    if("${THIS_LIB_TYPES}" MATCHES ";shared;")
        contest_add_basic_library(
            ${target} SHARED
            ${target}
            SOURCES ${THIS_SOURCES}
            INCLUDES ${THIS_INCLUDE_PATHS}
            LIBS ${THIS_LIBS}
        )
        set_target_properties(${target} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
        set_target_properties(${target} PROPERTIES  CXX_VISIBILITY_PRESET hidden)

        target_compile_definitions(${target} PRIVATE -D${target}_EXPORTS)
    endif()
endmacro()

# ex: contest_add_test(test_exe_name
#                      SOURCES sprite.cpp image.cpp ...
#                      LIBS catch dummy_lib ...)
macro(contest_add_exe target)
    cmake_parse_arguments(THIS "" "" "SOURCES;LIBS" ${ARGN})
    
    contest_stdafx_cpp()
    
    add_executable(${target} ${THIS_SOURCES})
    
    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    if(THIS_LIBS)
        target_link_libraries(${target} PRIVATE ${THIS_LIBS})
    endif()
    
    contest_wall(${target})
    contest_stdafx_h(${target})
endmacro()

# ex: contest_add_test(test_exe_name
#                      SOURCES sprite.cpp image.cpp ...
#                      LIBS catch dummy_lib
#                      TEST_DATA_DIR data)
macro (contest_add_test target)
    cmake_parse_arguments(THIS "" "" "SOURCES;LIBS;TEST_DATA_DIR" ${ARGN})
   
    contest_add_exe(${target} SOURCES ${THIS_SOURCES} LIBS ${THIS_LIBS})
   
    if(THIS_TEST_DATA_DIR)
        add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/${THIS_TEST_DATA_DIR} 
                $<TARGET_FILE_DIR:${target}>/test_data/${target}/${TEST_DATA_DIR})
    endif()
        
    add_test(${target} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${target})
endmacro()
