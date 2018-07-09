add_library(fplus INTERFACE)
target_include_directories(fplus INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

find_package(Threads REQUIRED)
target_link_libraries(fplus INTERFACE Threads::Threads)