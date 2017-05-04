#ifndef __SNN_NETWORK_H__
#define __SNN_NETWORK_H__

#include <boost/numpy.hpp>

namespace snn {

    namespace p = boost::python;
    namespace np = boost::numpy;

    np::ndarray find_fired(np::ndarray &);

} // namespace snn

#endif // __SNN_NETWORK_H__
