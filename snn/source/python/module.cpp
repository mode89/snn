#include <snn/python/network.h>

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;

    class_<snn::python::network>("network", init<int>())
        .def_readwrite("weight", &snn::python::network::m_weight)
        .def_readwrite("v", &snn::python::network::m_v)
        .def_readwrite("I", &snn::python::network::m_I)
        .add_property("fired", &snn::python::network::get_fired)
        .def("generate_random_input",
            &snn::python::network::generate_random_input)
        .def("find_fired_neurons",
            &snn::python::network::find_fired_neurons)
        .def("reset_fired_neurons",
            &snn::python::network::reset_fired_neurons)
        .def("process_firings", &snn::python::network::process_firings)
        .def("update_potentials", &snn::python::network::update_potentials)
    ;
}
