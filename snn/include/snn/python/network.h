#ifndef __SNN_PYTHON_NETWORK_H__
#define __SNN_PYTHON_NETWORK_H__

#include <boost/numpy.hpp>
#include <snn/network.h>

namespace snn { namespace python {

    namespace p = boost::python;
    namespace np = boost::numpy;

    np::ndarray find_fired(np::ndarray &);

    class network: public snn::network
    {
    private:
        // must be initialized before numpy objects
        p::object m_numpy_memory_owner;

    public:
        np::ndarray m_a;
        np::ndarray m_b;
        np::ndarray m_c;
        np::ndarray m_d;
        np::ndarray m_s;
        np::ndarray m_I;

    public:
        network(int N);
    };

}} // namespace snn::python

#endif // __SNN_PYTHON_NETWORK_H__
