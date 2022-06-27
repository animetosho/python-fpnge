import fpnge.binding
import fpnge.binding

def fromPIL(im): # : PIL.Image
	mode_map = {
	  "L":    (1, 8),
	  "RGB":  (3, 8),
	  "RGBA": (4, 8),
	  "PA":   (2, 8),
	  "RGBX": (4, 8),
	}
	if im.mode not in mode_map:
		conv_map = {
		  "1": "L",
		  "P": "RGBA",
		  "CMYK": "RGB",
		  "YCbCr": "RGB",
		  "LAB": "RGB",
		  "HSV": "RGB",
		  "LA": "RGBA",
		  "RGBa": "RGBA",
		  "La": "PA",
		}
		im = im.convert(mode=conv_map[im.mode])
	
	imbytes = im.tobytes()
	return fpnge.binding.encode_bytes(imbytes, im.width, im.height, *mode_map[im.mode])

def frombytes(bytes, width, height, channels, bits_per_channel, stride=0):
	return fpnge.binding.encode_bytes(**args)

def fromNP(ndarray): # : NumPy's ndarray
	if ndarray.ndim != 3:
		raise Exception("Must have 3 dimensions (width x height x channels)")
	return fpnge.binding.encode_view(ndarray.data, *ndarray.shape, ndarray.dtype.itemsize * 8)

def fromview(view, width=0, height=0, channels=0, bits_per_channel=0, stride=0):
	if stride == 0 and width == 0:
		stride = view.strides[0]
	if width == 0:
		width = view.shape[0]
	if height == 0:
		height = view.shape[1]
	if channels == 0:
		channels = view.shape[2]
	if bits_per_channel == 0:
		bits_per_channel = view.itemsize * 8
	return fpnge.binding.encode_view(view, width, height, channels, bits_per_channel, stride)

