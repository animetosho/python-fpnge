#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "fpnge.h"


PyObject* fpnge_encode(PyObject* self, PyObject* args) {
	(void)self;
	
	const char* data; Py_ssize_t data_len;
	unsigned width, stride, height, num_channels, bits_per_channel;
	
	if (!PyArg_ParseTuple(args, "y#IIIII", &data, &data_len, &width, &stride, &height, &num_channels, &bits_per_channel))
		return NULL;
	
	// check params
	if (width < 1 || height < 1) {
		PyErr_SetString(PyExc_ValueError, "Image size must be at least 1x1");
		return NULL;
	}
	if (num_channels < 1 || num_channels > 4) {
		PyErr_SetString(PyExc_ValueError, "Only 1-4 channels supported");
		return NULL;
	}
	if (bits_per_channel != 8 && bits_per_channel != 16) {
		PyErr_SetString(PyExc_ValueError, "Only 8/16 bits per channel supported");
		return NULL;
	}
	if (stride < width * num_channels * (bits_per_channel/8)) {
		PyErr_SetString(PyExc_ValueError, "Stride cannot be less than bytes per line");
		return NULL;
	}
	if (data_len < stride * height) {
		PyErr_SetString(PyExc_ValueError, "Not enough image data given");
		return NULL;
	}
	
	
	size_t output_len = FPNGEOutputAllocSize(bits_per_channel/8, num_channels, width, height);
	PyObject *Py_output_buffer = PyBytes_FromStringAndSize(NULL, output_len + 1);
	if (!Py_output_buffer) {
		PyErr_NoMemory();
		return NULL;
	}
	PyBytesObject *sv = (PyBytesObject *)Py_output_buffer;
	
	Py_BEGIN_ALLOW_THREADS; // TODO: do we need to add ref onto the input buffer?
	output_len = FPNGEEncode(bits_per_channel/8, num_channels, data, width, stride, height, sv->ob_sval);
	Py_END_ALLOW_THREADS;
	
	if (!output_len) {
		PyErr_SetString(PyExc_SystemError, "Failed to encode PNG");
		Py_DECREF(Py_output_buffer);
		return NULL;
	}
	
#if PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION < 9
	Py_SIZE(sv) = output_len;
#else
	Py_SET_SIZE(sv, output_len);
#endif
	sv->ob_sval[output_len] = '\0';
	// Reset hash, this was removed in Python 3.11
#if PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION < 11
	sv->ob_shash = -1;
#endif
	
	return Py_output_buffer;
}


static PyMethodDef fpnge_methods[] = {
	{
		"encode",
		fpnge_encode,
		METH_VARARGS,
		"encode(image_data, width, stride, height, num_channels, bits_per_channel)"
	},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef fpnge_definition = {
	PyModuleDef_HEAD_INIT,
	"fpnge.binding",
	"Python binding for fpnge",
	-1,
	fpnge_methods
};

PyMODINIT_FUNC PyInit_binding(void) {
	Py_Initialize();
	PyObject* module = PyModule_Create(&fpnge_definition);
	return module;
}
