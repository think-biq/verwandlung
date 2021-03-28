project(python_smallfbx)
set (PYBIND_MODULE_NAME python_smallfbx)

## Helper function to append parameters / options.
function(append_if condition value)
  if (${condition})
    foreach(variable ${ARGN})
      set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
    endforeach(variable)
  endif()
endfunction()

# Check if we can declare inline declarations as hidden, to unify clang visiblity.
# Thanks to https://github.com/Maroc-OS/retdec/pull/2/commits/0f90931787ee707a9579827b4869551cc4ab7de8
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
        CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
        CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-fvisibility-inlines-hidden" SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG)
    append_if(SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG "-fvisibility-inlines-hidden" CMAKE_CXX_FLAGS)
endif()

# Configre suffix naming convention for pyd library.
if (WIN32)
    set (pybind11_LIB_SUFFIX .cp39-win_amd64.pyd)
else ()
    set (pybind11_LIB_SUFFIX .cpython-39-darwin.so)
endif ()

set (PYBIND_LIBRARY_NAME "${PYBIND_MODULE_NAME}${pybind11_LIB_SUFFIX}")
if (WIN32)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set (python_smallfbx_PACKAGE_OUTPATH "${PROJECT_BINARY_DIR}/Debug/package")
	set (python_smallfbx_PYBIND_LIBRARY_FILEPATH "${PROJECT_BINARY_DIR}/Debug/${PYBIND_LIBRARY_NAME}")
else ()
	set (python_smallfbx_PACKAGE_OUTPATH "${PROJECT_BINARY_DIR}/Release/package")
	set (python_smallfbx_PYBIND_LIBRARY_FILEPATH "${PROJECT_BINARY_DIR}/Release/${PYBIND_LIBRARY_NAME}")
endif ()
else ()
	set (python_smallfbx_PACKAGE_OUTPATH "${PROJECT_BINARY_DIR}/package")
	set (python_smallfbx_PYBIND_LIBRARY_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_LIBRARY_NAME}")
endif ()

pybind11_add_module (${PYBIND_MODULE_NAME} "${PROJECT_SOURCE_DIR}/binding/module.cpp")
target_compile_definitions (${PYBIND_MODULE_NAME} PRIVATE PYBIND_MODULE_NAME=${PYBIND_MODULE_NAME})
target_include_directories (${PYBIND_MODULE_NAME} PRIVATE
	"${pybind11_INCLUDE_DIR}")
target_link_libraries (${PYBIND_MODULE_NAME}
	PUBLIC pybind11::module pybind11::lto pybind11::windows_extras)
# Copy package and binary module for distribution.
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/package"
            "${python_smallfbx_PACKAGE_OUTPATH}")
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${python_smallfbx_PYBIND_LIBRARY_FILEPATH}"
            "${python_smallfbx_PACKAGE_OUTPATH}/src")
# Then package.
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E chdir "${python_smallfbx_PACKAGE_OUTPATH}" ${PYTHON} setup.py bdist_wheel)


# Test.
add_custom_target(${PYBIND_MODULE_NAME}_test
     COMMAND ${PYTHON} -m pip install --force-reinstall "${python_smallfbx_PACKAGE_OUTPATH}"
     COMMAND ${PYTHON} "${PROJECT_SOURCE_DIR}/test/main.py" ${CMAKE_SOURCE_DIR}/etc/fbx/Gunan_animated.fbx
)
add_dependencies(${PYBIND_MODULE_NAME}_test ${PYBIND_MODULE_NAME})
