"""    
    Utility to inspect FBX files.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
"""
from .python_smallfbx import \
	FileVersion, \
	Object, ObjectClass, ObjectSubClass, \
	NodeAttribute, NullAttribute, RootAttribute, LimbNodeAttribute, LightAttribute, CameraAttribute, \
	Model, Null, Root, LimbNode, Mesh, Light, \
	CameraType, Camera, \
	AnimationKind, AnimationCurve, AnimationCurveNode, AnimationLayer, AnimationStack, \
	PropertyType, Property, \
	Node, \
	Document

__all__ = [
	'FileVersion',
	'Object', 'ObjectClass', 'ObjectSubClass',
	'NodeAttribute', 'NullAttribute', 'RootAttribute', 'LimbNodeAttribute', 'LightAttribute', 'CameraAttribute',
	'Model', 'Null', 'Root', 'LimbNode', 'Mesh', 'Light',
	'CameraType', 'Camera',
	'AnimationKind', 'AnimationCurve', 'AnimationCurveNode', 'AnimationLayer', 'AnimationStack',
	'PropertyType', 'Property',
	'Node',
	'Document'
]

_SmallFBX_Version = '2021.3.16'
_version = 4

def version():
	return f'{_SmallFBX_Version}.{_version}'