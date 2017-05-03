#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include <boost/python.hpp>
#include <iostream>
#include <numpy/arrayobject.h>

#undef NUMPY_IMPORT_ARRAY_RETVAL
#define NUMPY_IMPORT_ARRAY_RETVAL

class NumpyInitializer
{
public:
    NumpyInitializer()
    {
        import_array();
    }
};

static NumpyInitializer sNumpyInitializer;

void greet()
{
    std::cout << "Hello, World!" << std::endl;
}

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;
    def("greet", greet);
}
