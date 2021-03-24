CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
PROJECT(Verwandlung)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE PATH "" FORCE)
endif()

add_subdirectory(dep/zlib)

include_directories (
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX")
file(GLOB SMALLFBX_SRC 
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/*.h"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/*.h"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/*.cpp")
add_library(SmallFBX STATIC 
	${SMALLFBX_SRC})
target_include_directories(SmallFBX PRIVATE 
    "${PROJECT_SOURCE_DIR}/dep/zlib"
    "${PROJECT_SOURCE_DIR}/build/dep/zlib"
    ${SMALLFBX_SRC}
)
if(WIN32)
target_link_libraries (SmallFBX zlibstatic)
else()
target_link_libraries (SmallFBX z)
endif()

file (GLOB WANDEL_SRC
	"${PROJECT_SOURCE_DIR}/src/wandel/*.h"
	"${PROJECT_SOURCE_DIR}/src/wandel/*.cpp")
add_library (Wandel STATIC
	${WANDEL_SRC})
target_include_directories(Wandel PRIVATE 
    "${PROJECT_SOURCE_DIR}/dep/zlib"
    ${SMALLFBX_SRC}
	${WANDEL_SRC}
)
target_link_libraries (Wandel SmallFBX)
include_directories ("${PROJECT_SOURCE_DIR}/src/wandel")

add_executable (Verwandlung "${PROJECT_SOURCE_DIR}/src/cli/main.cpp")
target_link_libraries (Verwandlung Wandel)
set_target_properties(Verwandlung Wandel PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
target_include_directories(Verwandlung PRIVATE 
	${WANDEL_SRC}
    ${SMALLFBX_SRC}
)