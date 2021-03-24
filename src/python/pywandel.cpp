/*
    Utility to inspect FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <pybind11/pybind11.h>	// Base functions.
#include <pybind11/stl.h>		// C++ STL conversions.
#include <string>
#include <wandel.hpp>

#define SymbolToString(Symbol) #Symbol

namespace py = pybind11;
namespace wandel = biq::Verwandlung;

// Create pybind11 module. PYBIND_MODULE_NAME id defined in cmake script.
PYBIND11_MODULE(PYBIND_MODULE_NAME, m) {
	std::string desc = "Python module for ";
	desc += SymbolToString(PYBIND_MODULE_NAME);
	desc += " powered by the awesome pybind11 framework.";
	m.doc() = desc.c_str();

	// Expose WandleMode enum.
	py::enum_<wandel::WandelMode>(m, "WandelMode")
		.value("Unknown", wandel::WandelMode::Unknown)
		.value("List", wandel::WandelMode::List)
		.value("Export", wandel::WandelMode::Export)
		.export_values()
		;

	// Expose WandelParams config paramters struct.
	py::class_<biq::Verwandlung::WandelParams>(m, "WandelParams")
		.def(py::init())
		.def_readwrite("mode", &wandel::WandelParams::Mode)
		.def_readwrite("arguments", &wandel::WandelParams::Arguments)
		;

	// Expose main entry function for wandel library.
	m.def("Wandel", &wandel::Wandel, "Queries an FBX file and creates meta wandel data.");
}