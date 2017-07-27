#include <Python.h>

#include "ushuffle.h"

struct module_state {
    int l_;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *ushuffle_shuffle1(PyObject *self, PyObject *args) {
	const char *s;
	const int l;
	const int k;

        struct module_state *st = GETSTATE(self);

	/* example python call: ushuffle.shuffle1('string', 6, 2) */
	if (!PyArg_ParseTuple(args, "sii", &s, &l, &k)) {
		st->l_ = 0;
		return NULL;
	}

	st->l_ = l;
	shuffle1(s, l, k);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *ushuffle_shuffle2(PyObject *self, PyObject *args) {
	char *t;
	PyObject *T;

        struct module_state *st = GETSTATE(self);

	if (st->l_ <= 0)
		return NULL;

	if ((t = malloc((st->l_ + 1) * sizeof(char))) == NULL)
		return NULL;

	shuffle2(t);
	t[st->l_] = '\0';
	T = Py_BuildValue("s", t);
	free(t);
	return T;
}

static PyObject *ushuffle_shuffle(PyObject *self, PyObject *args) {
	const char *s;
	const int l;
	const int k;
	char *t;
	PyObject *T;

        struct module_state *st = GETSTATE(self);

	/* example python call: ushuffle.shuffle('string', 6, 2) */
	if (!PyArg_ParseTuple(args, "sii", &s, &l, &k))
		return NULL;
	st->l_ = l;
	if ((t = malloc((st->l_ + 1) * sizeof(char))) == NULL)
		return NULL;

	shuffle(s, t, l, k);
	t[st->l_] = '\0';
	T = Py_BuildValue("s", t);
	free(t);
	return T;
}

static PyMethodDef ushuffleMethods[] = {
    {"shuffle",  ushuffle_shuffle,  METH_VARARGS,
		"Calls shuffle1 and shuffle2."},
    {"shuffle1", ushuffle_shuffle1, METH_VARARGS,
		"Initialize a string for shuffling."},
	{"shuffle2", ushuffle_shuffle2, METH_VARARGS,
		"Return a new permutation."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "ushuffle",
        NULL,
        sizeof(struct module_state),
        ushuffleMethods,
};

PyMODINIT_FUNC
PyInit_ushuffle(void) {
    return
        PyModule_Create(&moduledef);
}
#else
PyMODINIT_FUNC initushuffle(void) {
    return
        Py_InitModule("ushuffle", ushuffleMethods);
}
#endif
