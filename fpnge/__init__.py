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
	return fpnge.binding.encode(imbytes, im.width, int(len(imbytes) / im.height), im.height, *mode_map[im.mode])

def frombytes(bytes, width, stride, height, channels, bits_per_channel):
	return fpnge.binding.encode(**args)
