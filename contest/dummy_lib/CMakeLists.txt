add_library(dummy_lib STATIC
	source/dummy.cpp
)

target_include_directories(dummy_lib
	PUBLIC "include"
)

target_link_libraries(dummy_lib PUBLIC Boost::boost)
