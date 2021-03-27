#.
#.	Utility to inspect FBX files.
#.
#.  2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
#.  See license.md for details.
#.
#.  https://think-biq.com

CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
PROJECT(Verwandlung)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE PATH "" FORCE)
endif()

# Check if we can declare inline declarations as hidden, to unify clang visiblity.
# Thanks to https://github.com/Maroc-OS/retdec/pull/2/commits/0f90931787ee707a9579827b4869551cc4ab7de8
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
		CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
		CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	function(append_if condition value)
	  if (${condition})
	    foreach(variable ${ARGN})
	      set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
	    endforeach(variable)
	  endif()
	endfunction()
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag("-fvisibility-inlines-hidden" SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG)
	append_if(SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG "-fvisibility-inlines-hidden" CMAKE_CXX_FLAGS)
endif()

include_directories ("${PROJECT_SOURCE_DIR}/dep/attr")

add_subdirectory (dep/zlib)

set (SMALLFBX_INCLUDE_DIR
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX")
file (GLOB SMALLFBX_SRC 
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/*.h"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/*.h"
	"${PROJECT_SOURCE_DIR}/dep/SmallFBX/src/SmallFBX/*.cpp")
add_library (SmallFBX STATIC 
	${SMALLFBX_SRC})
target_include_directories (SmallFBX PRIVATE 
    "${PROJECT_SOURCE_DIR}/dep/zlib"
    "${PROJECT_SOURCE_DIR}/build/dep/zlib"
    ${SMALLFBX_INCLUDE_DIR}
)
if (WIN32)
target_link_libraries (SmallFBX PUBLIC zlibstatic)
else ()
target_link_libraries (SmallFBX PUBLIC z)
endif ()

set (WANDEL_SRC_DIR "${PROJECT_SOURCE_DIR}/src/wandel")
set (WANDEL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/wandel")
file (GLOB WANDEL_SRC
	"${WANDEL_SRC_DIR}/*.h"
	"${WANDEL_SRC_DIR}/*.cpp")
add_library (WandelLib STATIC ${WANDEL_SRC})
#include_directories (${WANDEL_SRC_DIR})
target_include_directories (WandelLib PUBLIC 
    "${PROJECT_SOURCE_DIR}/dep/zlib"
    ${SMALLFBX_INCLUDE_DIR}
	${WANDEL_INCLUDE_DIR}
)
target_link_libraries (WandelLib PUBLIC SmallFBX)

# Build pybind11
add_subdirectory (dep/pybind11)

#[[# Configre suffix naming convention for pyd library.
if (WIN32)
	set (pybind11_LIB_SUFFIX .cp39-win_amd64.pyd)
else ()
    set (pybind11_LIB_SUFFIX .cpython-39-darwin.so)
endif ()]]

#[[set (PYTHON_MODULE_NAME )
set (PYBIND_MODULE_NAME ${PYBIND_WANDEL_MODULE_NAME})]]
#[[set (PYBIND_LIBRARY_NAME "${PYBIND_MODULE_NAME}${pybind11_LIB_SUFFIX}")
if (WIN32)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set (WANDEL_MODULE_OUTPATH "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PYBIND_MODULE_NAME}")
	set (WANDEL_PYBIND_LIBRARY_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/Debug/${PYBIND_LIBRARY_NAME}")
else ()
	set (WANDEL_MODULE_OUTPATH "${CMAKE_CURRENT_BINARY_DIR}/Release/${PYBIND_MODULE_NAME}")
	set (WANDEL_PYBIND_LIBRARY_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/Release/${PYBIND_LIBRARY_NAME}")
endif ()
else ()
	set (WANDEL_MODULE_OUTPATH "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_MODULE_NAME}")
	set (WANDEL_PYBIND_LIBRARY_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_LIBRARY_NAME}")
endif ()
pybind11_add_module (${PYBIND_MODULE_NAME} "${PROJECT_SOURCE_DIR}/src/${PYBIND_MODULE_NAME}/binding/module.cpp")
target_compile_definitions (${PYBIND_MODULE_NAME} PRIVATE PYBIND_MODULE_NAME=${PYBIND_MODULE_NAME})
target_include_directories (${PYBIND_MODULE_NAME} PRIVATE
	"${pybind11_INCLUDE_DIR}")
target_link_libraries (${PYBIND_MODULE_NAME}
	PUBLIC SmallFBX WandelLib pybind11::module pybind11::lto pybind11::windows_extras)
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/src/${PYBIND_MODULE_NAME}/package"
            "${WANDEL_MODULE_OUTPATH}")
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME}
    COMMAND ${CMAKE_COMMAND} -E echo ${WANDEL_PYBIND_LIBRARY_FILEPATH})
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${WANDEL_PYBIND_LIBRARY_FILEPATH}"
            "${WANDEL_MODULE_OUTPATH}/src/${PYBIND_LIBRARY_NAME}")]]


add_subdirectory (src/python_wandel)
target_include_directories (WandelLib PUBLIC	
    ${SMALLFBX_INCLUDE_DIR}
	${WANDEL_INCLUDE_DIR})
target_link_libraries (python_wandel
	PUBLIC SmallFBX WandelLib)


add_executable (Verwandlung "${PROJECT_SOURCE_DIR}/src/cli/main.cpp")
target_link_libraries (Verwandlung WandelLib)
set_target_properties (Verwandlung WandelLib PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
target_include_directories (Verwandlung PRIVATE 
	${WANDEL_SRC}
    ${SMALLFBX_SRC}
)