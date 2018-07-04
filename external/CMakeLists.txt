add_subdirectory(abseil)
add_subdirectory(boost-cmake)
add_subdirectory(cli11)

add_subdirectory(rapidjson)
add_subdirectory(json_dto)

add_subdirectory(stlab)
add_subdirectory(plog)
add_subdirectory(gsl)

add_subdirectory(gtest)
add_subdirectory(catch)
add_subdirectory(mpir)

if(NOT SUBMISSION_BUILD)
    add_subdirectory(sfml)
    add_subdirectory(imgui)
endif()





