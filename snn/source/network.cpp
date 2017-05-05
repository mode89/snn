#include <snn/network.h>

namespace snn {

    network::network(int N)
        : m_N(N)
        , m_Ne(N * 0.8)
        , m_Ni(m_N - m_Ne)
        , m_a(N)
        , m_b(N)
        , m_c(N)
        , m_d(N)
    {
        // initialize vector a
        for (int i = 0; i < m_Ne; ++ i)
            m_a[i] = 0.02;
        for (int i = m_Ne; i < m_N; ++ i)
            m_a[i] = 0.1;

        // initialize vector b
        for (int i = 0; i < m_N; ++ i)
            m_b[i] = 0.2;

        // initialize vector c
        for (int i = 0; i < m_N; ++ i)
            m_c[i] = -65.0;

        // initialize vector d
        for (int i = 0; i < m_Ne; ++ i)
            m_d[i] = 8.0;
        for (int i = m_Ne; i < m_N; ++ i)
            m_d[i] = 2.0;
    }

} // namespace snn
