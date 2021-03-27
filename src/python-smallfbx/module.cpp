/*
    Utility to inspect FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/

#include <pybind11/pybind11.h>	// Base functions.
#include <pybind11/stl.h>		// C++ STL conversions.
#include <string>

#include <sfbxDocument.h>

#define SymbolToString(Symbol) #Symbol

namespace py = pybind11;

// Create pybind11 module. PYBIND_MODULE_NAME id defined in cmake script.
PYBIND11_MODULE(PYBIND_MODULE_NAME, m) {

	{
		std::string desc = "Python module for ";
		desc += SymbolToString(PYBIND_MODULE_NAME);
		desc += " powered by the awesome pybind11 framework.";
		m.doc() = desc.c_str();
	}

	// Expose WandleMode enum.
	py::enum_<sfxb::FileVersion>(m, "FileVersion")
		.value("Fbx2014", sfxb::FileVersion::Fbx2014)
		.value("Fbx2015", sfxb::FileVersion::Fbx2015)
		.value("Fbx2016", sfxb::FileVersion::Fbx2016)
		.value("Fbx2017", sfxb::FileVersion::Fbx2017)
		.value("Fbx2018", sfxb::FileVersion::Fbx2018)
		.value("Fbx2019", sfxb::FileVersion::Fbx2019)
		.value("Fbx2020", sfxb::FileVersion::Fbx2020)
		.value("Default", sfxb::FileVersion::Default)
		.export_values()
		;

	//
	py::class_<sfbx::Document>(m, "Document")
		.def(py::init<std::string>()) // Empty default constructor.
		.def("isValid", &sfbx::Document::isValid)
		.def("getVersion", &sfbx::Document::getVersion)
		;

#if 0
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
#endif
}