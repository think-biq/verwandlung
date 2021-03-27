"""
	So much change!
"""
import io
from contextlib import redirect_stdout
from io import StringIO
from .python_wandel import Wandel as wandel_raw
from .python_wandel import WandelMode, WandelParams

__all__ = ['wandel', 'WandelMode', 'WandelParams']

def version():
	return "1.0.2"

def wandel(filepath, params):
	f = io.StringIO()

	with redirect_stdout(f):
	    wandel_raw(filepath, params)

	return f.getvalue()