cmake_minimum_required(VERSION 3.2)

if (POLICY CMP0058)
        # Ninja requires custom command byproducts to be explicit
        cmake_policy(SET CMP0058 NEW)
endif()

project(contest-template)
set (CMAKE_CXX_STANDARD 14)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(ucm)
include(cotire)
include(contest)

set(out_dir "${CMAKE_BINARY_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${out_dir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${out_dir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${out_dir}")

set(external_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (UNIX)
    ucm_add_flags(-pthread)
    ucm_add_flags(CXX -fexceptions  -ftemplate-backtrace-limit=0)
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
    ucm_add_flags(CXX -Wall)
endif()

 if(APPLE)
        ucm_add_flags(-I/opt/X11/include)
        ucm_add_linker_flags(EXE -L/opt/X11/lib -lglut)
endif()

set(BOOST_DOWNLOAD_TO_BINARY_DIR OFF CACHE BOOL "download to binary")
set(BOOST_IOSTREAMS_ENABLE_BZIP2 OFF CACHE BOOL "use bzip2")

set(SFML_BUILD_NETWORK OFF CACHE BOOL "build network")
set(SFML_BUILD_AUDIO OFF CACHE BOOL "build audio")

add_subdirectory(external)
add_subdirectory(contest)

enable_testing()
add_subdirectory(tests)

set(submission_dir submission)
set(submission_archive submission.tar.gz)

add_custom_command(OUTPUT ${submission_archive}
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${submission_dir}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${submission_dir}"
    
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${external_dir}" "${submission_dir}/external"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/contest" "${submission_dir}/contest"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests" "${submission_dir}/tests"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${submission_dir}/cmake"
    
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${submission_dir}/external/boost-cmake/boost"
    
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" "${submission_dir}"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build.sh" "${submission_dir}"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Makefile" "${submission_dir}"
    
    COMMAND ${CMAKE_COMMAND} -E chdir "${submission_dir}" tar czf "../${submission_archive}" .
)

add_custom_target(pack_submission DEPENDS ${submission_archive})
