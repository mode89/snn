#include <snn/network.h>

BOOST_PYTHON_MODULE(snn)
{
    using namespace boost::python;
    def("find_fired", snn::find_fired);
}
