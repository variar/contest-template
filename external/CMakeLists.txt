add_subdirectory(boost-cmake)
add_subdirectory(tclap)
add_subdirectory(rapidjson)

add_subdirectory(stlab)
add_subdirectory(lemon)
add_subdirectory(plog)

add_subdirectory(gtest)
add_subdirectory(catch)

if(NOT SUBMISSION_BUILD)
    add_subdirectory(sfml)
    add_subdirectory(sfgui)
endif()





