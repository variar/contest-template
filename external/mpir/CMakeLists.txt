project (mpir-generic C)

cmake_minimum_required (VERSION 2.8.6)

include_directories("./")
include_directories("./fft")
include_directories("./mpf")
include_directories("./mpq")
include_directories("./mpn/generic")
include_directories("./mpz")
include_directories("./printf")
include_directories("./scanf")

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

file(
	GLOB public_headers
	"./*.h"
)

add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D_LIB)
if(WIN32)
	add_definitions(-D_WIN64) #usually building for x64
endif()

add_library(mpir STATIC ${mpir_SRC})

target_include_directories(mpir INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


