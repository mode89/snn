#include <snn/python/network.h>

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;

    class_<snn::python::network>("network", init<int>());
}
