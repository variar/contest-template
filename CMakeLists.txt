cmake_minimum_required(VERSION 3.2)

project(contest-template)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ucm.cmake)

set (CMAKE_CXX_STANDARD 11)

set(out_dir "${CMAKE_BINARY_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${out_dir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${out_dir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${out_dir}")

set(external_dir ${CMAKE_CURRENT_SOURCE_DIR}/external)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (UNIX)
    ucm_add_flags(-lpthread)
    ucm_add_linker_flags(EXE -lpthread)
endif()

if (APPLE)
    ucm_add_flags(-I/opt/X11/include)
    ucm_add_linker_flags(EXE -L/opt/X11/lib -lglut)
endif()

set(BOOST_DOWNLOAD_TO_BINARY_DIR ON CACHE BOOL "download to binary")
set(BOOST_IOSTREAMS_ENABLE_BZIP2 OFF CACHE BOOL "use bzip2")

set(SFML_BUILD_NETWORK OFF CACHE BOOL "build network")
set(SFML_BUILD_AUDIO OFF CACHE BOOL "build audio")

set(TBB_BUILD_TESTS OFF)
set(TBB_BUILD_SHARED OFF)
set(TBB_BUILD_TBBMALLOC OFF)
set(TBB_BUILD_TBBMALLOC_PROXY OFF)

add_subdirectory(external)
add_subdirectory(contest)
add_subdirectory(tests)
