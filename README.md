This simple module provides Python bindings for the fast PNG encoder, [fpnge](https://github.com/animetosho/fpnge).

# Installation

This module has only been tested with Python3. Distutils is required (`apt install python3-distutils` under Debian based distros should grab it).

fpnge currently only runs on x86 processors, and requires SSE4.1+PCLMUL support at minimum.
Note: there is no dynamic CPU detection implemented, so this module should be installed/compiled on the machine it is going to run on.

This module can then be installed by running `python3 setup.py install` in the folder with the code.

# Example Usage

Exporting a PIL.Image to a PNG file.

```python
from PIL import Image
import fpnge

with Image.open('input.png') as im:
	png = fpnge.fromPIL(im)
	with open('output.png', 'wb') as f:
		f.write(png)
```

API
===

Note: fpnge does not support indexed color or bit depths below 8 bits/pixel.

fpnge.fromPIL(image)
------------------------------------------------------------------

Converts a PIL image specified in *image* to a PNG, returning it as a bytes object. If the image uses a palette, it will be converted to RGBA before saving.

fpnge.frombytes(bytes, width, stride, height, channels, bits_per_channel)
--------------------------------------------------------------------------------

Converts a raw image specified as a bytes object in *bytes* to a PNG, returning it as a bytes object.

The pixel dimensions of the supplied image must be specified in *width* and *height,* with *stride* being the number of bytes per row (usually is *width* \* *channels* \* *bits_per_channel*/8).
*bits_per_channel* signifies the bit depth for each colour channel. Only 8 and 16 bits per channel are supported.
*channels* refers to the number of colour channels the image has, with the following assumption:

* 1 channel: this is a greyscale image
* 2 channels: this is a greyscale + alpha image
* 3 channels: RGB colour
* 4 channels: RGBA colour

No other values are allowed for *channels*

License
=======

This module is Public Domain or [CC0](https://creativecommons.org/publicdomain/zero/1.0/legalcode) (or equivalent) if PD isn’t recognised.

fpnge itself is licensed under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)