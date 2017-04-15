# python_sdbox
Python wrapper to optimization routine SDBOX

This repository contains a Python wrapper to SDBOX, an open source derivative-free optimization solver (available for download at http://www.dis.uniroma1.it/~lucidi/DFL/).

For more information, see here: https://lindonjroberts.wordpress.com/2017/04/15/writing-c-extensions-for-python-with-numpy-and-callback-functions/

It does not contain a copy of the actual SDBOX routine (see link above), which must be compiled separately.

After compiling SDBOX, to build the Python wrapper, run

    python setup.py build_ext --inplace

(for Python 2.x; replace 'python' with 'python3' for Python 3.x)

Included is an example of how to use the wrapper (minimizing the 2d Rosenbrock function: the true minimum is f(1,1)=0). Run this with

    python test_sdbox.py

Available under the GNU GPL License v3.0, with no warranty (implied or otherwise) for any purpose.
