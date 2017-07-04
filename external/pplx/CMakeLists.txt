set(PPLX_SRC_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(PPLX_SOURCES_COMMON
 ${PPLX_SRC_DIR}/pplx/pplx.cpp
 ${PPLX_SRC_DIR}/utilities/asyncrt_utils.cpp
)

if(UNIX)
  set(PPLX_SOURCES
    ${PPLX_SOURCES_COMMON}
    ${PPLX_SRC_DIR}/pplx/threadpool.cpp
  )
  if(APPLE)
    list(APPEND PPLX_SOURCES
      ${PPLX_SRC_DIR}/pplx/pplxapple.cpp
    )
  else()
    list(APPEND PPLX_SOURCES 
		${PPLX_SRC_DIR}/pplx/pplxlinux.cpp
	)
  endif()
elseif(WIN32)
  set(PPLX_SOURCES
  	${PPLX_SRC_DIR}/pch/stdafx.cpp
    ${PPLX_SOURCES_COMMON}
    ${PPLX_SRC_DIR}/pplx/pplxwin.cpp
 )

 if (BUILD_SHARED_LIBS)
    add_definitions(-D_ASYNCRT_EXPORT -D_PPLX_EXPORT -D_USRDLL)
  endif()
endif()

add_library(pplx STATIC ${PPLX_SOURCES})

set_target_properties(pplx PROPERTIES
        COTIRE_CXX_PREFIX_HEADER_INIT "${PPLX_SRC_DIR}/pch/stdafx.h"
        COTIRE_ADD_UNITY_BUILD FALSE
    )

target_link_libraries(pplx
    Boost::thread
    Boost::system
)
target_compile_definitions(pplx PUBLIC -D_NO_PPLXIMP -D_NO_ASYNCRTIMP -DBOOST_SYSTEM_NO_DEPRECATED)
target_include_directories(pplx 
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${PPLX_SRC_DIR}/pch
)
cotire(pplx)
