#include <boost/python.hpp>
#include <iostream>

void greet()
{
    std::cout << "Hello, World!" << std::endl;
}

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;
    def("greet", greet);
}
