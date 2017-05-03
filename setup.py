from setuptools import setup, Extension
import numpy.distutils

setup(
    name="snn",
    ext_modules = [
        Extension(
            name="snn",
            sources=[
                "snn.cpp"
            ],
            libraries=[
                "boost_python-3.4"
            ],
            include_dirs=numpy.distutils.misc_util.get_numpy_include_dirs()
        ),
    ],
)
