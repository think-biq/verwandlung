set (python_smallfbx_MODULE python_smallfbx)

# Extract project version from source
file(STRINGS "${SMALLFBX_INCLUDE_DIR}/SmallFBX/sfbxTokens.h"
     python_smallfbx_RAW_VERSION REGEX "define sfbxS_SmallFBXWithVersion   (SmallFBXVersion)")
message(STATUS "Extracted version: ${python_smallfbx_RAW_VERSION}")

# Project declaration.
project(
  ${python_smallfbx_MODULE}
  LANGUAGES CXX
  VERSION "2021.3.16"
)
message(STATUS "pybin11 project for module ${python_smallfbx_MODULE}")

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
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
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

# Ensure build path access on unix-y and windows platforms.
set (PYBIND_LIBRARY_NAME "${python_smallfbx_MODULE}${pybind11_LIB_SUFFIX}")
if (WIN32)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set (python_smallfbx_PACKAGE_BUILD_DIR
    "${PROJECT_BINARY_DIR}/Debug/package")
	set (python_smallfbx_LIBRARY_PATH "${PROJECT_BINARY_DIR}/Debug/${PYBIND_LIBRARY_NAME}")
else ()
	set (python_smallfbx_PACKAGE_BUILD_DIR "${PROJECT_BINARY_DIR}/Release/package")
	set (python_smallfbx_LIBRARY_PATH "${PROJECT_BINARY_DIR}/Release/${PYBIND_LIBRARY_NAME}")
endif ()
else ()
	set (python_smallfbx_PACKAGE_BUILD_DIR "${PROJECT_BINARY_DIR}/package")
	set (python_smallfbx_LIBRARY_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_LIBRARY_NAME}")
endif ()

set (python_smallfbx_INC_DIR
  "${PROJECT_SOURCE_DIR}/binding"
)
set (python_smallfbx_SRC_DIR
  "${python_smallfbx_INC_DIR}"
)
file (GLOB python_smallfbx_SRC
  "${python_smallfbx_SRC_DIR}/*.h"
  "${python_smallfbx_SRC_DIR}/*.cpp"
)

# Define pybind module, based on the previously set module name, and set
# path to the main modulue definition file.
pybind11_add_module (${python_smallfbx_MODULE}
  "${python_smallfbx_SRC_DIR}/module.cpp"
)
# Strip the target build, to avoid linker confusion about weak symbol links.
# I.E.: ld: warning: direct access in function 
#  'std::__1::__shared_ptr_pointer<sfbx::Object*, std::__1::default_delete<sfbx::Object>, std::__1::allocator<sfbx::Object> >
#               ::__get_deleter(std::type_info const&) const' 
# from file 'CMakeFiles/python_smallfbx.dir/binding/module.cpp.o'
# to global weak symbol 'typeinfo name for std::__1::default_delete<sfbx::Object>'
# from file '../../libSmallFBX.a(sfbxDocument.cpp.o)' means the weak symbol cannot
# be overridden at runtime. This was likely caused by different translation units
# being compiled with different visibility settings.
pybind11_strip (${python_smallfbx_MODULE})
# Setup the module name for resolution in main module definition file.
target_compile_definitions (${python_smallfbx_MODULE} PRIVATE
  PYBIND_MODULE_NAME=${python_smallfbx_MODULE}
)
target_include_directories (${python_smallfbx_MODULE} PRIVATE
  "${python_smallfbx_INC_DIR}"
  "${pybind11_INCLUDE_DIR}"
)
target_link_libraries (${python_smallfbx_MODULE} PUBLIC
  pybind11::module
  pybind11::lto
  pybind11::windows_extras
)
# Copy package and binary module for distribution.
add_custom_command(
  TARGET
    ${python_smallfbx_MODULE} POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/package"
    "${python_smallfbx_PACKAGE_BUILD_DIR}"
)
add_custom_command(
  TARGET
    ${python_smallfbx_MODULE} POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${python_smallfbx_LIBRARY_PATH}"
    "${python_smallfbx_PACKAGE_BUILD_DIR}/src"
)
# Then package.
add_custom_command(
  TARGET
    ${python_smallfbx_MODULE} POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E chdir "${python_smallfbx_PACKAGE_BUILD_DIR}" ${PYTHON} setup.py bdist_wheel
)

# Create dedicated test target by force reinstalling the newly created library,
# then running the entry point of the test package.
add_custom_target(${python_smallfbx_MODULE}_test
  COMMAND
    ${PYTHON} -m pip install --force-reinstall "${python_smallfbx_PACKAGE_BUILD_DIR}"
  COMMAND
    ${PYTHON} "${PROJECT_SOURCE_DIR}/test" ${CMAKE_SOURCE_DIR}/etc/fbx/Gunan_animated.fbx
)
add_dependencies(${python_smallfbx_MODULE}_test ${python_smallfbx_MODULE})
