find_package( OpenGL REQUIRED )

add_library(imgui STATIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui-SFML.cpp")

target_include_directories(imgui SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(imgui SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui")

target_link_libraries(imgui PUBLIC sfml-system sfml-window sfml-graphics ${OPENGL_gl_LIBRARY})

set_target_properties(imgui PROPERTIES FOLDER "external")

set_target_properties(imgui PROPERTIES 
    COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.h"
    COTIRE_COTIRE_ADD_UNITY_BUILD FALSE
	FOLDER "imgui"
)
cotire(imgui)
