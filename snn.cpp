#include <boost/numpy.hpp>
#include <iostream>

namespace np = boost::numpy;

class NumpyInitializer
{
public:
    NumpyInitializer()
    {
        np::initialize();
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
