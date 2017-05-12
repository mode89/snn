#include <snn/python/network.h>

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;

    class_<snn::python::network>("network", init<int, int>())
        .def_readonly("a", &snn::python::network::m_a)
        .def_readonly("b", &snn::python::network::m_b)
        .def_readonly("c", &snn::python::network::m_c)
        .def_readonly("d", &snn::python::network::m_d)
        .def_readwrite("s", &snn::python::network::m_s)
        .def_readwrite("v", &snn::python::network::m_v)
        .def_readwrite("u", &snn::python::network::m_u)
        .def_readwrite("I", &snn::python::network::m_I)
        .add_property("fired", &snn::python::network::get_fired)
        .def("generate_random_input",
            &snn::python::network::generate_random_input)
        .def("find_fired_neurons",
            &snn::python::network::find_fired_neurons)
        .def("reset_fired_neurons",
            &snn::python::network::reset_fired_neurons)
        .def("process_firings", &snn::python::network::process_firings)
        .def("deliver_delayed_spikes",
            &snn::python::network::deliver_delayed_spikes)
        .def("update_potentials", &snn::python::network::update_potentials)
    ;
}
