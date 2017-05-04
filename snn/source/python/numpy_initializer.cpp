#include <boost/numpy.hpp>

namespace snn {

    namespace np = boost::numpy;

    class NumpyInitializer
    {
    public:
        NumpyInitializer()
        {
            np::initialize();
        }
    };

    static NumpyInitializer sNumpyInitializer;

} // namespace snn
