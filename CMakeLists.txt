CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(Verwandlung)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE PATH "" FORCE)
endif()

find_package(ZLIB REQUIRED)
link_directories("C:\\Program Files\\zlib\\lib")
#set_target_properties(ZLIB PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)

include_directories (
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/")
file(GLOB SMALLFBX_SRC 
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/*.h"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/*.h"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/*.cpp")
add_library(SmallFBX STATIC 
	${SMALLFBX_SRC})
target_include_directories(SmallFBX PRIVATE 
    ${ZLIB_INCLUDE_DIRS}
    ${SMALLFBX_SRC}
)
target_link_libraries (SmallFBX zlibstatic)
#set_target_properties(SmallFBX PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
#install(TARGETS SmallFBX)


# hannelore core
file (GLOB WANDEL_SRC
	"${PROJECT_SOURCE_DIR}/src/wandel/*.h"
	"${PROJECT_SOURCE_DIR}/src/wandel/*.cpp")
add_library (Wandel STATIC
	${WANDEL_SRC})
#target_link_libraries (Wandel SmallFBX)
target_include_directories(Wandel PRIVATE 
    ${SMALLFBX_SRC}
	${WANDEL_SRC}
)
target_link_libraries (Wandel SmallFBX)
include_directories ("${PROJECT_SOURCE_DIR}/src/wandel")
#set_target_properties(Wandel PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
#install(TARGETS Wandel)

add_executable (Verwandlung "${PROJECT_SOURCE_DIR}/src/cli/main.cpp")
target_link_libraries (Verwandlung Wandel)
set_target_properties(Verwandlung Wandel PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
target_include_directories(Verwandlung PRIVATE 
	${WANDEL_SRC}
    ${SMALLFBX_SRC}
)