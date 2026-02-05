#include <Python.h>
#include <stdlib.h>
#include <stdio.h>

extern unsigned char _embedded_pyc[];
extern unsigned int _embedded_pyc_len;

int main(int argc,char *argv[]) {
    Py_SetProgrammName(argv[0]);
    Py_Initialize();

    PyObject *code = PyMarshal_ReadObjectFromString(
        (char *)_embedded_pyc,
        _embedded_pyc_len
    );

    if (code != NULL) {
        PyObject *module = PyImport_AddModule("__main__");
        PyObject *dict = PyModule_GetDict(module);

        PyObject *result = PyEval_EvalCode(code,dict,dict);

        if (result == NULL) {
            PyErr_Print();
            Py_Finalize();
            return 1;
        }

        Py_DECREF(result);
    } else {
        PyErr_Print();
        Py_Finalize();
        return 1;
    }

    Py_Finalize();
    return 0;
}