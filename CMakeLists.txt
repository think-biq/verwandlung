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

include_directories (
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
    ${SMALLFBX_SRC}
)
if (WIN32)
target_link_libraries (SmallFBX PUBLIC zlibstatic)
else ()
target_link_libraries (SmallFBX PUBLIC z)
endif ()

set (WANDEL_SRC_DIR "${PROJECT_SOURCE_DIR}/src/wandel")
file (GLOB WANDEL_SRC
	"${WANDEL_SRC_DIR}/*.h"
	"${WANDEL_SRC_DIR}/*.cpp")
add_library (WandelLib STATIC ${WANDEL_SRC})
include_directories (${WANDEL_SRC_DIR})
target_include_directories (WandelLib PRIVATE 
    "${PROJECT_SOURCE_DIR}/dep/zlib"
    ${SMALLFBX_SRC}
	${WANDEL_SRC}
)
target_link_libraries (WandelLib PUBLIC SmallFBX)

set (PYBIND_MODULE_NAME wandel)
execute_process(COMMAND python3-config --extension-suffix
                OUTPUT_VARIABLE pybind11_LIB_SUFFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
set (PYBIND_LIBRARY_NAME "${PYBIND_MODULE_NAME}${pybind11_LIB_SUFFIX}")
add_definitions (-DPYBIND_MODULE_NAME=${PYBIND_MODULE_NAME})
add_subdirectory (dep/pybind11)
include_directories (
	"${PROJECT_SOURCE_DIR}/dep/pybind11/include")
pybind11_add_module (${PYBIND_MODULE_NAME} "${PROJECT_SOURCE_DIR}/src/python/pywandel.cpp")
target_link_libraries (${PYBIND_MODULE_NAME} PUBLIC SmallFBX WandelLib pybind11::module pybind11::lto pybind11::windows_extras)
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/python
            ${CMAKE_CURRENT_BINARY_DIR}/wandel)
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_LIBRARY_NAME}"
            "${CMAKE_CURRENT_BINARY_DIR}/wandel/src/wandel_cli")
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm
            "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_LIBRARY_NAME}")
execute_process(COMMAND ls -lav "${CMAKE_CURRENT_BINARY_DIR}/wandel/setup.py" bdist_wheel)

add_executable (Verwandlung "${PROJECT_SOURCE_DIR}/src/cli/main.cpp")
target_link_libraries (Verwandlung WandelLib)
set_target_properties (Verwandlung WandelLib PROPERTIES COMPILE_DEFINITIONS BUILDER_STATIC_DEFINE)
target_include_directories (Verwandlung PRIVATE 
	${WANDEL_SRC}
    ${SMALLFBX_SRC}
)