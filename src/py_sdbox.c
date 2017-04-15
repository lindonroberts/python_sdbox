#include <Python.h>
#include <numpy/arrayobject.h>
#include "headers.h" // header file for sdbox library

/*
 * The solver is:
 *      void sdbox(int n, double *lb, double *ub, double *x0, double objfun(int, double*), double *xmin, double *fmin, int maxiter, double eps)
 * where
 *       n = dimension
 *      lb = lower bounds array (size n)
 *      ub = upper bounds array (size n)
 *     xin = starting point (size n)
 *  objfun = objective function (double -> double)
 *   [xmin = solution x (size n)]
 *   [fmin = solution f(x)]
 * maxiter = maximum number of main loop iterations
 *     eps = convergence tolerance
*/

static PyObject* python_objfun;  // global variable with callback function

double objfun(int n, double *x) {
    double fx;
    // Turn x into a numpy array, then into 1-tuple, then evaluate my_callback_np
    npy_intp dims[1];
    dims[0] = n;
    PyObject* x_np = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, x);
    PyObject *arglist = Py_BuildValue("(O)", x_np);
    PyObject *result = PyEval_CallObject(python_objfun, arglist);
    fx = PyFloat_AsDouble(result);
    Py_XDECREF(x_np);
    Py_XDECREF(arglist);
    Py_XDECREF(result);
    return fx;
}


// Python interface:
//      x, f = sdbox(objfun, x0, lb, ub, maxiter, eps)

static PyObject* sdbox_wrapper(PyObject *self, PyObject *args) {
    PyObject *obj;
    PyArrayObject *x0_array, *lb_array, *ub_array;
    int maxiter, n;
    double eps;
    double *x0, *lb, *ub;
    
    if (!PyArg_ParseTuple(args, "OO!O!O!id", &obj, &PyArray_Type, &x0_array, &PyArray_Type, &lb_array, &PyArray_Type, &ub_array, &maxiter, &eps)) {
        return NULL;
    }
    
    // Set objective function to global variable
    if (!PyCallable_Check(obj)) {
        PyErr_SetString(PyExc_TypeError, "first parameter must be callable");
        return NULL;
    }
    Py_XINCREF(obj);
    Py_XDECREF(python_objfun);
    python_objfun = obj;
    Py_XDECREF(obj);
    
    // Check x0 dimensions and type, set n
    if (PyArray_NDIM(x0_array) != 1 || PyArray_TYPE(x0_array) != NPY_DOUBLE) {
        PyErr_SetString(PyExc_ValueError, "x0 must be one-dimensional array of type float");
        return NULL;
    }
    n = (int)PyArray_DIM(x0_array, 0);
    x0 = (double*)PyArray_DATA(x0_array);
    
    // Check lb dimensions and type
    if (PyArray_NDIM(lb_array) != 1 || PyArray_TYPE(lb_array) != NPY_DOUBLE) {
        PyErr_SetString(PyExc_ValueError, "lb must be one-dimensional array of type float");
        return NULL;
    }
    if ((int)PyArray_DIM(lb_array, 0) != n) {
        PyErr_SetString(PyExc_ValueError, "lb must be the same length as x0");
        return NULL;
    }
    lb = (double*)PyArray_DATA(lb_array);
    
    // Check ub dimensions and type
    if (PyArray_NDIM(ub_array) != 1 || PyArray_TYPE(ub_array) != NPY_DOUBLE) {
        PyErr_SetString(PyExc_ValueError, "ub must be one-dimensional array of type float");
        return NULL;
    }
    if ((int)PyArray_DIM(ub_array, 0) != n) {
        PyErr_SetString(PyExc_ValueError, "ub must be the same length as x0");
        return NULL;
    }
    ub = (double*)PyArray_DATA(ub_array);
    
    // Define outputs and call SDBOX
    double *xmin;
    xmin = (double *)malloc(n);
    double fmin;
    
    sdbox(n, lb, ub, x0, objfun, xmin, &fmin, maxiter, eps);
    
    // Wrap up results and return
    npy_intp dims[1];
    dims[0] = n;
    PyObject* xmin_object = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, xmin);
    return Py_BuildValue("(Od)", xmin_object, fmin);
}


/*  define functions in module */
static PyMethodDef MyMethods[] =
{
     {"sdbox", sdbox_wrapper, METH_VARARGS, "call optimization solver SDBOX"},
     {NULL, NULL, 0, NULL}
};


/* Module Initialization: different in Python 2.x and 3.x */
#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT, "pysdbox", 0, -1, MyMethods
};

PyMODINIT_FUNC PyInit_pysdbox(void) {
    import_array();  // required for numpy compatibility
    return PyModule_Create(&module);
};

#else

PyMODINIT_FUNC initpysdbox(void)
{
     (void) Py_InitModule("pysdbox", MyMethods);
     import_array(); // required for numpy compatibility
}

#endif

