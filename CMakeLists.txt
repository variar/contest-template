cmake_minimum_required(VERSION 3.2)

if (POLICY CMP0058)
        # Ninja requires custom command byproducts to be explicit
        cmake_policy(SET CMP0058 NEW)
endif()

project(contest-template)
set (CMAKE_CXX_STANDARD 14)

set(out_dir "${CMAKE_BINARY_DIR}/output")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${out_dir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${out_dir}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${out_dir}")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(ucm)
include(cotire)
include(contest)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
endif()

if (UNIX)
    ucm_add_flags(-pthread)
    ucm_add_flags(CXX -fexceptions  -ftemplate-backtrace-limit=0)
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
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
add_subdirectory(templates)

enable_testing()
add_subdirectory(tests)

contest_add_submission(submission submission.tar.gz exe_template submission_exe)


