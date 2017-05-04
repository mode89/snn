#ifndef __SNN_PYTHON_NETWORK_H__
#define __SNN_PYTHON_NETWORK_H__

#include <boost/numpy.hpp>
#include <snn/network.h>

namespace snn { namespace python {

    namespace np = boost::numpy;

    np::ndarray find_fired(np::ndarray &);

    class network: public snn::network
    {
    public:
        network(int N);
    };

}} // namespace snn::python

#endif // __SNN_PYTHON_NETWORK_H__
