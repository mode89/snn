from setuptools import setup, Extension

setup(
    name="snn",
    ext_modules = [
        Extension(
            name="snn",
            sources=[
                "snn.cpp"
            ],
            libraries=[
                "boost_python-3.4",
                "boost_numpy",
            ],
        ),
    ],
)
