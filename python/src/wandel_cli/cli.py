import io
from contextlib import redirect_stdout
from io import StringIO
from .pywandel import Wandel as wandel_raw
from .pywandel import WandelMode, WandelParams

def wandel(filepath, params):
	f = io.StringIO()

	with redirect_stdout(f):
	    wandel_raw(filepath, params)

	return f.getvalue()