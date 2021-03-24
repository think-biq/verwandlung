#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <wandel.hpp>

#define SymbolToString(Symbol) #Symbol

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

PYBIND11_MODULE(PYBIND_MODULE_NAME, m) {
	std::string desc = "Python module for ";
	desc += SymbolToString(PYBIND_MODULE_NAME);
	desc += " powered by the awesome pybind11 framework.";
	m.doc() = desc.c_str(); // optional module docstring

	m.def("Wandel", &biq::Verwandlung::Wandel, "Wandels your shit!");

	py::enum_<biq::Verwandlung::WandelMode>(m, "WandelMode")
		.value("Unknown", biq::Verwandlung::WandelMode::Unknown)
		.value("List", biq::Verwandlung::WandelMode::List)
		.value("Export", biq::Verwandlung::WandelMode::Export)
		.export_values()
		;

	py::class_<biq::Verwandlung::WandelParams>(m, "WandelParams")
		.def(py::init())
		.def_readwrite("mode", &biq::Verwandlung::WandelParams::Mode)
		.def_readwrite("arguments", &biq::Verwandlung::WandelParams::Arguments)
		;
}