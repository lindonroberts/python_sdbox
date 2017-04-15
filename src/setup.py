from distutils.core import setup, Extension

# To compile:
#   [python/python3] setup.py build_ext --inplace

# Details of the SDBOX library
sdbox_libname = 'sdbox'  # looking for lib[name].so
sdbox_libdir = '.'

# Wrapper info
libname = 'pysdbox'  # Python library name
libsrc = 'py_sdbox.c'  # C wrapper source file

# Compiler flags (gcc -D) - check using up-to-date NumPy API
defs = []
defs.append(('NPY_NO_DEPRECATED_API', 'NPY_1_11_API_VERSION'))

# Define & build module
pysdbox = Extension(libname, sources=[libsrc], library_dirs=[sdbox_libdir], libraries=[sdbox_libname],
        define_macros=defs)
setup(ext_modules=[pysdbox])

