set(P7_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Sources)

add_library(p7 STATIC 
    ${P7_SRC}/ClBaical.cpp
    ${P7_SRC}/ClFile.cpp
    ${P7_SRC}/Client.cpp
    ${P7_SRC}/ClNull.cpp
    ${P7_SRC}/ClText.cpp
    ${P7_SRC}/CRC32.cpp
    ${P7_SRC}/Proxy.cpp
    ${P7_SRC}/Telemetry.cpp
    ${P7_SRC}/Trace.cpp
)

target_include_directories(p7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Headers)
target_include_directories(p7 PRIVATE ${P7_SRC})
target_include_directories(p7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Shared)

if (WIN32)
    target_include_directories(p7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Shared/Platforms/Windows_x86)
else()
    target_include_directories(p7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Shared/Platforms/Linux_x86)
    target_link_libraries(p7 INTERFACE pthread rt)
endif()



