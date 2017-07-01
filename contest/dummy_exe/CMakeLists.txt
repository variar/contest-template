
add_executable(dummy_exe
	source/main.cpp
)

target_link_libraries(dummy_exe
	dummy_lib
	sfgui
	tclap
	Boost::boost
	Boost::program_options
	tbb_static
)
