#!/usr/bin/python3

# Lindon Roberts, 2017

"""
Wrapper gives the interface:
    x, f = sdbox(objfun, x0, lb, ub, maxiter, eps)
where
    objfun = objective function (float -> float)
    x0 = initial point (numpy array)
    lb, ub = lower & upper bounds (lb <= x <= ub)
    maxiter = number of main loop iterations to run
    eps = tolerance for grid size in direct search

To compile everything:
    1. Compile SDBOX shared library "libsdbox.so" (run 'make')
    2. Compile Python wrapper "pysdbox.so" (python setup.py build_ext --inplace)
    3. Run this file as a test

Wrapper: Lindon Roberts, 2017

Details of solver:
SDBOX - C and F90 implementation of a Derivative-Free algorithm for bound constrained optimization problems
Copyright (C) 2011  G.Liuzzi
Available from http://www.dis.uniroma1.it/~lucidi/DFL/

Reference:
S. Lucidi, M. Sciandrone. A Derivative-Free Algorithm for Bound Constrained Optimization, 
Computational Optimization and Applications, 21(2): 119-142 (2002) DOI: 10.1023/A:1013735414984
"""


import numpy as np
from pysdbox import sdbox


def rosenbrock(x):
    m = 2
    fi = np.zeros((m,))
    fi[0] = 10.0*(x[1]-x[0]**2)
    fi[1] = 1.0-x[0]
    return fi


def rosenbrock_objfun(x):
    fx = np.sum(rosenbrock(x)**2)
    #print "Objective is %g at x = %s" % (fx, str(x))
    return fx

if __name__ == '__main__':
    x0 = np.array([-1.2, 1])
    lb = -100.0*np.ones(x0.shape)
    ub = 100.0*np.ones(x0.shape)
    maxiter = 3000  # this is the main determinant of how long it runs for
    eps = 1e-8

    x, f = sdbox(rosenbrock_objfun, x0, lb, ub, maxiter, eps)
    print("SDBOX found the minimum f = %g at x = %s" % (f, str(x)))

