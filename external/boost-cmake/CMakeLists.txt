cmake_minimum_required(VERSION 3.0.0)
project(Boost-CMake)

option(BOOST_DOWNLOAD_TO_BINARY_DIR "Prefer downloading Boost to the binary directory instead of source directory" OFF)
set(BOOST_URL "https://s3.eu-central-1.amazonaws.com/icfp2018/boost_1_67_0.tar.xz" CACHE STRING "Boost download URL")
set(BOOST_URL_SHA256 "68d22a2b3b133eae98611359e8340f100a081e88dcb03afe3226c40b0842ebb5" CACHE STRING "Boost download URL SHA256 checksum")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(DownloadBoost)

message(STATUS "Boost found: ${BOOST_VERSION} ${BOOST_SOURCE}")

include(StandaloneBuild)
include(PlatformDetect)
include(AddBoostLib)
include(AddBoostTest)

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

if(USE_ANDROID)
  # CMake 3.7.1 doesn't define the target triple for the ASM language,
  # resulting in all files compiled for the host architecture
  set(CMAKE_ASM_COMPILER_TARGET "${CMAKE_CXX_COMPILER_TARGET}")
endif()

set(BOOST_LIBS
  # Header only libs
  header

  # Compiled libs
  atomic
  chrono
  container
  #context
  #coroutine
  #date_time
  exception
  #fiber
  filesystem
  graph
  #iostreams
  locale
  #log
  #math
  #mpi
  #graph_parallel # depends on mpi, so needs to be put after it
  program_options
  #python # complex module
  #random
  regex
  serialization
  #signals
  system
  #unit_test_framework
  thread
  #timer
  #type_erasure
  #wave
)

foreach(lib ${BOOST_LIBS})
  include("libs/${lib}.cmake")
endforeach()

# TODO: Move those to option() calls in the right file
if(NOT BOOST_STANDALONE)
  # Compilation options required by all platforms
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS $<$<CONFIG:Release>:BOOST_DISABLE_ASSERT>)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_SYSTEM_NO_DEPRECATED)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_VERSION=4)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_USES_CHRONO)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_PROVIDES_EXECUTORS)
endif()

if(USE_ANDROID)
  # Android doesn't support thread local storage through compiler intrinsics
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_ASIO_DISABLE_THREAD_KEYWORD_EXTENSION)
endif()
