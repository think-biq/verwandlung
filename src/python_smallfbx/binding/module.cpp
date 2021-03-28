/*
	Utility to inspect FBX files.

	2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
	See license.md for details.

	https://think-biq.com
*/

#include <pybind11/pybind11.h>	// Base functions.
#include <pybind11/stl.h>		// C++ STL conversions.
//#include <pybind11/numpy.h>
#include <string>
#include <memory>
#include <iostream>

#include <sfbxRawVector.h>
#include <sfbxObject.h>
#include <sfbxAnimation.h>
#include <sfbxDocument.h>

#define SymbolToString(Symbol) #Symbol

namespace py = pybind11;

template <typename TContainer, typename TElement>
std::function<std::vector<TElement> (py::object& self)> make_span_as_vector(sfbx::span<TElement> (TContainer::*f)(void) const) {
	return [f](py::object& self) {
		auto& obj = self.cast<TContainer&>();
		const auto& span = (obj.*f)();
		return std::vector<TElement>(span.begin(), span.end());
	};
}


#define SFBX_PYCLASS(Context, Class, Parent) \
	py::class_<sfbx::Class, std::shared_ptr<sfbx::Class>, sfbx::Parent>(Context, #Class)

#define SFBX_PYCLASS_OBJ(Context, Class) \
	SFBX_PYCLASS(Context, Class, Object)

// Create pybind11 module. PYBIND_MODULE_NAME id defined in cmake script.
PYBIND11_MODULE(PYBIND_MODULE_NAME, m) {

	{
		std::string desc = "Python module for ";
		desc += SymbolToString(PYBIND_MODULE_NAME);
		desc += " powered by the awesome pybind11 framework.";
		m.doc() = desc.c_str();
	}

	// Expose FileVersion enum.
	py::enum_<sfbx::FileVersion>(m, "FileVersion")
		.value("Fbx2014", sfbx::FileVersion::Fbx2014)
		.value("Fbx2015", sfbx::FileVersion::Fbx2015)
		.value("Fbx2016", sfbx::FileVersion::Fbx2016)
		.value("Fbx2017", sfbx::FileVersion::Fbx2017)
		.value("Fbx2018", sfbx::FileVersion::Fbx2018)
		.value("Fbx2019", sfbx::FileVersion::Fbx2019)
		.value("Fbx2020", sfbx::FileVersion::Fbx2020)
		.value("Default", sfbx::FileVersion::Default)
		.export_values()
		;
	
	py::class_<sfbx::Object, std::shared_ptr<sfbx::Object>>(m, "Object")
		.def("id", &sfbx::Object::getID)
		.def("name", &sfbx::Object::getName)
		.def("full_name", &sfbx::Object::getFullName)
		;

	SFBX_PYCLASS_OBJ(m, AnimationLayer)
		;
	
	SFBX_PYCLASS_OBJ(m, AnimationStack)
		.def("start_time_local", &sfbx::AnimationStack::getLocalStart)
		.def("stop_time_local", &sfbx::AnimationStack::getLocalStop)
		.def("start_time_ref", &sfbx::AnimationStack::getReferenceStart)
		.def("stop_time_ref", &sfbx::AnimationStack::getReferenceStop)
		.def("animation_layers", make_span_as_vector(&sfbx::AnimationStack::getAnimationLayers))
		;

	// Expose top-level class of library, representing a FBX document.
	py::class_<sfbx::Document>(m, "Document")
		.def(py::init<std::string>()) // Select filepath constructor.
		.def("valid", &sfbx::Document::valid)
		.def("version", &sfbx::Document::getVersion)
		.def("animation_stacks", make_span_as_vector(&sfbx::Document::getAnimationStacks))
		;
}