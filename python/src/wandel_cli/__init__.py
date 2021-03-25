import io
from contextlib import redirect_stdout
from io import StringIO 
from .wandel import Wandel as wandel_raw
from .wandel import WandelMode, WandelParams

__all__ = ['wandel', 'WandelMode', 'WandelParams']

def wandel(filepath, params):
	f = io.StringIO()

	with redirect_stdout(f):
	    wandel_raw(filepath, params)

	return f.getvalue()