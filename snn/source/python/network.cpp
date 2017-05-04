#include <snn/python/network.h>

namespace snn { namespace python {

    namespace p = boost::python;

    np::ndarray find_fired(np::ndarray & npV)
    {
        const double * vData = reinterpret_cast<double*>(npV.get_data());

        std::vector<int> fired;
        for (int i = 0; i < 1000; ++ i)
            if (vData[i] >= 30.0)
                fired.push_back(i);

        np::ndarray npFired = np::empty(
            p::make_tuple(fired.size()),
            np::dtype::get_builtin<int>());
        int * npFiredData = reinterpret_cast<int*>(npFired.get_data());
        for (int i = 0; i < fired.size(); ++ i)
            npFiredData[i] = fired[i];

        return npFired;
    }

    network::network(int N)
        : snn::network(N)
    {
    }

}} // namespace snn::python
