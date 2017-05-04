#include <snn/python/network.h>

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;
    def("find_fired", snn::python::find_fired);

    class_<snn::python::network>("network", init<int>());
}
