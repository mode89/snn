#ifndef __SNN_PYTHON_NETWORK_H__
#define __SNN_PYTHON_NETWORK_H__

#include <boost/numpy.hpp>
#include <snn/network.h>

namespace snn { namespace python {

    namespace p = boost::python;
    namespace np = boost::numpy;

    class network: public snn::network
    {
    private:
        // must be initialized before numpy objects
        p::object m_numpy_memory_owner;

    public:
        np::ndarray m_s;
        np::ndarray m_v;
        np::ndarray m_I;

    public:
        np::ndarray get_fired();

    public:
        network(int N);
    };

}} // namespace snn::python

#endif // __SNN_PYTHON_NETWORK_H__
