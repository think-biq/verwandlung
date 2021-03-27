project(python_wandel)


# Configre suffix naming convention for pyd library.
if (WIN32)
    set (pybind11_LIB_SUFFIX .cp39-win_amd64.pyd)
else ()
#[[execute_process(COMMAND PYTHON tools/python3-config --extension-suffix
                OUTPUT_VARIABLE pybind11_LIB_SUFFIX OUTPUT_STRIP_TRAILING_WHITESPACE)]]
    set (pybind11_LIB_SUFFIX .cpython-39-darwin.so)
endif ()

#[[set (PYTHON_MODULE_NAME )
set (PYBIND_MODULE_NAME ${PYBIND_WANDEL_MODULE_NAME})]]
set (PYBIND_LIBRARY_NAME "python_wandel${pybind11_LIB_SUFFIX}")
if (WIN32)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set (python_wandel_PACKAGE_OUTPATH "${PROJECT_BINARY_DIR}/Debug/package")
	set (python_wandel_PYBIND_LIBRARY_FILEPATH "${PROJECT_BINARY_DIR}/Debug/${PYBIND_LIBRARY_NAME}")
else ()
	set (python_wandel_PACKAGE_OUTPATH "${PROJECT_BINARY_DIR}/Release/package")
	set (python_wandel_PYBIND_LIBRARY_FILEPATH "${PROJECT_BINARY_DIR}/Release/${PYBIND_LIBRARY_NAME}")
endif ()
else ()
	set (python_wandel_PACKAGE_OUTPATH "${PROJECT_BINARY_DIR}/package")
	set (python_wandel_PYBIND_LIBRARY_FILEPATH "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_LIBRARY_NAME}")
endif ()

set (${PYBIND_MODULE_NAME} python_wandel)
pybind11_add_module (${PYBIND_MODULE_NAME} "${PROJECT_SOURCE_DIR}/binding/module.cpp")
target_compile_definitions (${PYBIND_MODULE_NAME} PRIVATE PYBIND_MODULE_NAME=${PYBIND_MODULE_NAME})
target_include_directories (${PYBIND_MODULE_NAME} PRIVATE
	"${pybind11_INCLUDE_DIR}")
target_link_libraries (${PYBIND_MODULE_NAME}
	PUBLIC pybind11::module pybind11::lto pybind11::windows_extras)
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/package"
            "${python_wandel_PACKAGE_OUTPATH}")
add_custom_command(
    TARGET ${PYBIND_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${python_wandel_PYBIND_LIBRARY_FILEPATH}"
            "${python_wandel_PACKAGE_OUTPATH}/src")