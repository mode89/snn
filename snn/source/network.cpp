#include <snn/network.h>

namespace snn {

    network::network(int N)
        : m_N(N)
        , m_Ne(N * 0.8)
        , m_Ni(m_N - m_Ne)
    {
    }

} // namespace snn
