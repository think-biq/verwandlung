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
add_library (WandelLib STATIC
	${WANDEL_SRC})
target_include_directories(WandelLib PRIVATE 
    "${PROJECT_SOURCE_DIR}/dep/zlib"
    ${SMALLFBX_SRC}
	${WANDEL_SRC}
)
target_link_libraries (WandelLib SmallFBX)
include_directories ("${PROJECT_SOURCE_DIR}/src/wandel")

set(PYBIND_MODULE_NAME wandel)
add_definitions(-DPYBIND_MODULE_NAME=${PYBIND_MODULE_NAME})
add_subdirectory(dep/pybind11)
include_directories (
	"${PROJECT_SOURCE_DIR}/dep/pybind11/include")
pybind11_add_module(${PYBIND_MODULE_NAME} "${PROJECT_SOURCE_DIR}/src/python/pywandel.cpp")
target_link_libraries(${PYBIND_MODULE_NAME} PRIVATE pybind11::module pybind11::lto pybind11::windows_extras WandelLib)

add_executable (Verwandlung "${PROJECT_SOURCE_DIR}/src/cli/main.cpp")
target_link_libraries (Verwandlung WandelLib)
set_target_properties(Verwandlung WandelLib PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
target_include_directories(Verwandlung PRIVATE 
	${WANDEL_SRC}
    ${SMALLFBX_SRC}
)