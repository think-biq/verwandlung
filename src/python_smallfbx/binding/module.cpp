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
#include <functional>

#include <SmallFBX/sfbxRawVector.h>
#include <SmallFBX/sfbxObject.h>
#include <SmallFBX/sfbxAnimation.h>
#include <SmallFBX/sfbxDocument.h>
#include <SmallFBX/sfbxModel.h>
#include <SmallFBX/sfbxNode.h>
#include <SmallFBX/sfbxProperty.h>

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

#define RAW_PYCLASS(Context, Class) \
	py::class_<sfbx::Class>(Context, #Class)

#define SFBX_PYCLASS(Context, Class, Parent) \
	py::class_<sfbx::Class, std::shared_ptr<sfbx::Class>, sfbx::Parent>(Context, #Class)

#define SFBX_PYCLASS_OBJ(Context, Class) \
	SFBX_PYCLASS(Context, Class, Object)

#define SFBX_ENUM(Context, Enum) \
	py::enum_<sfbx::Enum>(Context, #Enum)
#define SFBX_ENUM_VALUE(Enum, Value, DocStr) \
	value(#Value, sfbx::Enum::Value, DocStr)

// Create pybind11 module. PYBIND_MODULE_NAME id defined in cmake script.
PYBIND11_MODULE(PYBIND_MODULE_NAME, m) {

	{
		std::string desc = "Python module for ";
		desc += SymbolToString(PYBIND_MODULE_NAME);
		desc += " powered by the awesome pybind11 framework.";
		m.doc() = desc.c_str();
	}

	// Document.h

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
	

	// Object.h

	// Object
	py::class_<sfbx::Object, std::shared_ptr<sfbx::Object>>(m, "Object")
		.def("id", &sfbx::Object::getID)
		.def("name", &sfbx::Object::getName)
		.def("full_name", &sfbx::Object::getFullName)
		.def("parents", make_span_as_vector(&sfbx::Object::getParents))
		.def("children", make_span_as_vector(&sfbx::Object::getChildren))
		;

	//
	SFBX_ENUM(m, ObjectClass)
	.SFBX_ENUM_VALUE(ObjectClass, Unknown, "Unknown")
    .SFBX_ENUM_VALUE(ObjectClass, NodeAttribute, "NodeAttribute")
    .SFBX_ENUM_VALUE(ObjectClass, Model, "Model")
    .SFBX_ENUM_VALUE(ObjectClass, Geometry, "Geometry")
    .SFBX_ENUM_VALUE(ObjectClass, Deformer, "Deformer")
    .SFBX_ENUM_VALUE(ObjectClass, Pose, "Pose")
    .SFBX_ENUM_VALUE(ObjectClass, Video, "Video")
    .SFBX_ENUM_VALUE(ObjectClass, Texture, "Texture")
    .SFBX_ENUM_VALUE(ObjectClass, Material, "Material")
    .SFBX_ENUM_VALUE(ObjectClass, AnimationStack, "AnimationStack")
    .SFBX_ENUM_VALUE(ObjectClass, AnimationLayer, "AnimationLayer")
    .SFBX_ENUM_VALUE(ObjectClass, AnimationCurveNode, "AnimationCurveNode")
    .SFBX_ENUM_VALUE(ObjectClass, AnimationCurve, "AnimationCurve")
    .SFBX_ENUM_VALUE(ObjectClass, Implementation, "Implementation")
    .SFBX_ENUM_VALUE(ObjectClass, BindingTable, "BindingTable")
    ;

	//
	SFBX_ENUM(m, ObjectSubClass)
	.SFBX_ENUM_VALUE(ObjectSubClass, Unknown, "Unknown")
    .SFBX_ENUM_VALUE(ObjectSubClass, Null, "Null")
    .SFBX_ENUM_VALUE(ObjectSubClass, Root, "Root")
    .SFBX_ENUM_VALUE(ObjectSubClass, LimbNode, "LimbNode")
    .SFBX_ENUM_VALUE(ObjectSubClass, Light, "Light")
    .SFBX_ENUM_VALUE(ObjectSubClass, Camera, "Camera")
    .SFBX_ENUM_VALUE(ObjectSubClass, Mesh, "Mesh")
    .SFBX_ENUM_VALUE(ObjectSubClass, Shape, "Shape")
    .SFBX_ENUM_VALUE(ObjectSubClass, BlendShape, "BlendShape")
    .SFBX_ENUM_VALUE(ObjectSubClass, BlendShapeChannel, "BlendShapeChannel")
    .SFBX_ENUM_VALUE(ObjectSubClass, Skin, "Skin")
    .SFBX_ENUM_VALUE(ObjectSubClass, Cluster, "Cluster")
    .SFBX_ENUM_VALUE(ObjectSubClass, BindPose, "BindPose")
    .SFBX_ENUM_VALUE(ObjectSubClass, Clip, "Clip")
    ;


	// Model.h

	//
	SFBX_PYCLASS_OBJ(m, NodeAttribute);
	SFBX_PYCLASS_OBJ(m, NullAttribute);
	SFBX_PYCLASS_OBJ(m, RootAttribute);
	SFBX_PYCLASS_OBJ(m, LimbNodeAttribute);
	SFBX_PYCLASS_OBJ(m, LightAttribute);
	SFBX_PYCLASS_OBJ(m, CameraAttribute);

	SFBX_PYCLASS_OBJ(m, Model);
	SFBX_PYCLASS_OBJ(m, Null);
	SFBX_PYCLASS_OBJ(m, Root);
	SFBX_PYCLASS_OBJ(m, LimbNode);
	SFBX_PYCLASS_OBJ(m, Mesh);
	SFBX_PYCLASS_OBJ(m, Light);

	SFBX_ENUM(m, CameraType)
		.SFBX_ENUM_VALUE(CameraType, Perspective, "perspective")
		.SFBX_ENUM_VALUE(CameraType, Orthographic, "Orthographic")
		;
	SFBX_PYCLASS_OBJ(m, Camera);


	// Animation.h

	//
	SFBX_ENUM(m, AnimationKind)
		.SFBX_ENUM_VALUE(AnimationKind, Unknown, "unknown")
		// transform
		.SFBX_ENUM_VALUE(AnimationKind, Position, "float3")
		.SFBX_ENUM_VALUE(AnimationKind, Rotation, "float3")
		.SFBX_ENUM_VALUE(AnimationKind, Scale, "float3")
		// light
		.SFBX_ENUM_VALUE(AnimationKind, Color, "light color -> float3")
		.SFBX_ENUM_VALUE(AnimationKind, Intensity, "light intensity -> float3")
		// camera
		.SFBX_ENUM_VALUE(AnimationKind, FocalLength, "camera focal length, float, in mm")
		.SFBX_ENUM_VALUE(AnimationKind, FilmWidth, "camera film width, float, in mm (unit converted. raw value is inch")
		.SFBX_ENUM_VALUE(AnimationKind, FilmHeight, "camera film height, float, in mm (unit converted. raw value is inch")
		.SFBX_ENUM_VALUE(AnimationKind, FilmOffsetX, "camera film offsetx, float, in mm (unit converted. raw value is inch")
		.SFBX_ENUM_VALUE(AnimationKind, FilmOffsetY, "camera film offsety, float, in mm (unit converted. raw value is inch")
		// blend shape
		.SFBX_ENUM_VALUE(AnimationKind, DeformWeight, "float, 0.0-1.0 (unit converted. raw value is percent")
		;


	SFBX_PYCLASS_OBJ(m, AnimationCurve)
		;

	SFBX_PYCLASS_OBJ(m, AnimationCurveNode)
		.def("kind", &sfbx::AnimationCurveNode::getAnimationKind)
		.def("target", &sfbx::AnimationCurveNode::getAnimationTarget, py::return_value_policy::reference_internal)
		.def("curves", make_span_as_vector(&sfbx::AnimationCurveNode::getAnimationCurves))
		;

	SFBX_PYCLASS_OBJ(m, AnimationLayer)
		.def("curve_nodes", make_span_as_vector(&sfbx::AnimationLayer::getAnimationCurveNodes))
		;
	
	SFBX_PYCLASS_OBJ(m, AnimationStack)
		.def("start_time_local", &sfbx::AnimationStack::getLocalStart)
		.def("stop_time_local", &sfbx::AnimationStack::getLocalStop)
		.def("start_time_ref", &sfbx::AnimationStack::getReferenceStart)
		.def("stop_time_ref", &sfbx::AnimationStack::getReferenceStop)
		.def("layers", make_span_as_vector(&sfbx::AnimationStack::getAnimationLayers))
		;


	// Property.h	

	//
	SFBX_ENUM(m, PropertyType)
		.SFBX_ENUM_VALUE(PropertyType, Unknown, "Unknown")
		.SFBX_ENUM_VALUE(PropertyType, Bool, "Bool")
		.SFBX_ENUM_VALUE(PropertyType, Int16, "Int16")
		.SFBX_ENUM_VALUE(PropertyType, Int32, "Int32")
		.SFBX_ENUM_VALUE(PropertyType, Int64, "Int64")
		.SFBX_ENUM_VALUE(PropertyType, Float32, "Float32")
		.SFBX_ENUM_VALUE(PropertyType, Float64, "Float64")
		.SFBX_ENUM_VALUE(PropertyType, String, "String")
		.SFBX_ENUM_VALUE(PropertyType, Blob, "Blob")
		.SFBX_ENUM_VALUE(PropertyType, BoolArray, "BoolArray")
		.SFBX_ENUM_VALUE(PropertyType, Int16Array, "Int16Array")
		.SFBX_ENUM_VALUE(PropertyType, Int32Array, "Int32Array")
		.SFBX_ENUM_VALUE(PropertyType, Int64Array, "Int64Array")
		.SFBX_ENUM_VALUE(PropertyType, Float32Array, "Float32Array")
		.SFBX_ENUM_VALUE(PropertyType, Float64Array, "Float64Array")
		;

	//
	RAW_PYCLASS(m, Property);


	// Node.h	

	//
	RAW_PYCLASS(m, Node)
		.def("name", &sfbx::Node::getName)
		//TODO: why? //.def("properties", make_span_as_vector(&sfbx::Node::getProperties)) ???
		.def("children", make_span_as_vector(&sfbx::Node::getChildren))		
		.def("parent", &sfbx::Node::getParent, py::return_value_policy::reference_internal)
		.def("__str__", &sfbx::Node::toString)
		;


	// Expose top-level class of library, representing a FBX document.
	py::class_<sfbx::Document>(m, "Document")
		.def(py::init<std::string>()) // Select filepath constructor.
		.def("valid", &sfbx::Document::valid)
		.def("version", &sfbx::Document::getVersion)
		.def("stacks", make_span_as_vector(&sfbx::Document::getAnimationStacks))
		.def("objects", make_span_as_vector(&sfbx::Document::getAllObjects))
		.def("root_objects", make_span_as_vector(&sfbx::Document::getRootObjects))
		.def("root_model", &sfbx::Document::getRootModel, py::return_value_policy::reference_internal)
		.def("nodes", make_span_as_vector(&sfbx::Document::getAllNodes))
		.def("root_nodes", make_span_as_vector(&sfbx::Document::getRootNodes))
		;
}