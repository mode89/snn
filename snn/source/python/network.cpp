#include <snn/python/network.h>

namespace snn { namespace python {

    namespace p = boost::python;

    network::network(int N)
        : snn::network(N)
    {
    }

}} // namespace snn::python
