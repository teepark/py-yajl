/*
 * Copyright 2009, R. Tyler Ballance <tyler@monkeypox.org>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 * 
 *  3. Neither the name of R. Tyler Ballance nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */ 
#include <Python.h>

#include "py_yajl.h"

static PyMethodDef yajldecoder_methods[] = {
    {"decode", (PyCFunction)(py_yajldecoder_decode), METH_VARARGS, NULL},
    {"raw_decode", (PyCFunction)(py_yajldecoder_raw_decode), METH_VARARGS, NULL},
    {NULL}
};

static PyMethodDef yajlencoder_methods[] = {
    {"encode", (PyCFunction)(py_yajlencoder_encode), METH_VARARGS, NULL},
    {NULL}
};

static PyTypeObject YajlDecoderType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "yajl.YajlDecoder",        /*tp_name*/
    sizeof(_YajlDecoder),      /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)yajldecoder_dealloc,       /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Yajl-based decoder",      /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    yajldecoder_methods,  /* tp_methods */
    NULL,                 /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)(yajldecoder_init),/* tp_init */
    0,                         /* tp_alloc */
}; 

static PyTypeObject YajlEncoderType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "yajl.YajlEncoder",        /*tp_name*/
    sizeof(_YajlEncoder),      /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)yajlencoder_dealloc,       /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Yajl-based encoder",      /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    yajlencoder_methods,  /* tp_methods */
    NULL,                 /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)(yajlencoder_init),/* tp_init */
    0,                         /* tp_alloc */
}; 

PyObject *__decode = NULL;
PyObject *__encode = NULL;

static PyObject *py_loads(PYARGS)
{
    PyObject *decoder = NULL;
    PyObject *str = NULL;
    PyObject *result = NULL;

    if (!PyArg_ParseTuple(args, "O", &str)) {
        return NULL;
    }
    
    decoder = PyObject_Call((PyObject *)(&YajlDecoderType), NULL, NULL);
    if (decoder == NULL) {
        return NULL;
    }

    if (__decode == NULL)
        __decode = PyUnicode_FromString("decode");

    result = PyObject_CallMethodObjArgs(decoder, __decode, str, NULL);
    Py_XDECREF(decoder);
    return result;
}

static PyObject *py_dumps(PYARGS)
{
    PyObject *encoder = NULL;
    PyObject *obj = NULL;
    PyObject *result = NULL;

    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }

    encoder = PyObject_Call((PyObject *)(&YajlEncoderType), NULL, NULL);
    if (encoder == NULL) {
        return NULL;
    }

    if (__encode == NULL)
        __encode = PyUnicode_FromString("encode");

    result = PyObject_CallMethodObjArgs(encoder, __encode, obj, NULL);
    Py_XDECREF(encoder);
    return result;
}

static struct PyMethodDef yajl_methods[] = {
    {"dumps", (PyCFunction)(py_dumps), METH_VARARGS, NULL},
    {"loads", (PyCFunction)(py_loads), METH_VARARGS, NULL},
    {NULL}
};

static struct PyModuleDef yajlmodule = {
    PyModuleDef_HEAD_INIT,
    "yajl",
    NULL,
    -1,
    yajl_methods,
    NULL,
    NULL,
    NULL,
    NULL
};



PyMODINIT_FUNC PyInit_yajl(void)
{
		PyObject *module = PyModule_Create(&yajlmodule);

    YajlDecoderType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&YajlDecoderType) < 0)
        return NULL;

    Py_INCREF(&YajlDecoderType);
    PyModule_AddObject(module, "Decoder", (PyObject *)(&YajlDecoderType));

    YajlEncoderType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&YajlEncoderType) < 0)
        return NULL;

    Py_INCREF(&YajlEncoderType);
    PyModule_AddObject(module, "Encoder", (PyObject *)(&YajlEncoderType));

		return module;
}

