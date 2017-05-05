#include <snn/python/network.h>

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;

    class_<snn::python::network>("network", init<int>())
        .def_readonly("a", &snn::python::network::m_a)
        .def_readonly("b", &snn::python::network::m_b)
        .def_readonly("c", &snn::python::network::m_c)
        .def_readonly("d", &snn::python::network::m_d)
        .def_readwrite("s", &snn::python::network::m_s)
        .def_readwrite("I", &snn::python::network::m_I)
    ;
}
