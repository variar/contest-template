
add_executable(dummy_exe
	  ${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.cpp
	  ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
)

target_include_directories(dummy_exe PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(dummy_exe
	dummy_lib
	sfgui
	tclap
	Boost::boost
	Boost::program_options
	tbb_static
	stlab
)

set_target_properties(dummy_exe PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/include/stdafx.h")
cotire(dummy_exe)
