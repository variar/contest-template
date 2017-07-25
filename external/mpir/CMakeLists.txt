project (mpir-generic C)

cmake_minimum_required (VERSION 2.8.6)

file(
	GLOB mpir_SRC
	"./*.h"
	"./*.c"
	"./fft/*.c"
	"./mpf/*.c"
	"./mpn/generic/*.c"
	"./mpq/*.c"
	"./mpz/*.c"
	"./printf/*.c"
	"./scanf/*.c"
)

add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D_LIB)
if(WIN32)
	add_definitions(-D_WIN64) #usually building for x64
endif()

add_library(mpir STATIC ${mpir_SRC})

target_include_directories(mpir SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(mpir PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/fft
        ${CMAKE_CURRENT_SOURCE_DIR}/mpf
        ${CMAKE_CURRENT_SOURCE_DIR}/mpq
        ${CMAKE_CURRENT_SOURCE_DIR}/mpn/generic
        ${CMAKE_CURRENT_SOURCE_DIR}/mpz
        ${CMAKE_CURRENT_SOURCE_DIR}/printf
        ${CMAKE_CURRENT_SOURCE_DIR}/scanf)


