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
        , m_s(N, N)
        , m_v(N)
        , m_u(N)
        , m_I(N)
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

        // initialize matrix of synaptic strength
        for (int i = 0; i < m_N; ++ i)
        {
            for (int j = 0; j < m_Ne; ++ j)
                m_s(i, j) = 0.25;
            for (int j = m_Ne; j < m_N; ++ j)
                m_s(i, j) = -0.5;
        }

        // initialize vectors v and u
        m_v = m_c;
        m_u = m_b % m_v;
    }

    void network::update_potentials()
    {
        m_v += ((0.04 * m_v + 5.0) % m_v + 140.0 - m_u + m_I);
        m_u += (m_a % (m_b % m_v - m_u));
    }

} // namespace snn
