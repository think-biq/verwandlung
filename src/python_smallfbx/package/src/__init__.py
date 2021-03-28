"""
	So much change!
"""
from .python_smallfbx import Document

__all__ = ['Document']

_SmallFBX_Version = '2021.3.16'
_version = 4

def version():
	return f'{_SmallFBX_Version}.{_version}'