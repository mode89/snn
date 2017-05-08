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
        , m_fired(N)
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

        // reserved memory for firings
        m_fired.reserve(m_N);
    }

    void network::generate_random_input()
    {
        std::uniform_real_distribution<double> dist(-6.5, 6.5);
        for (int i = 0; i < m_N; ++ i)
            m_I[i] = dist(m_random_engine);
    }

    void network::process_firings()
    {
        m_fired.clear();

        // detect fired neurons
        for (int i = 0; i < m_N; ++ i)
            if (m_v[i] >= 30.0)
                m_fired.push_back(i);

        // reset potential of fired neurons
        for (int i, n = m_fired.size(); i < n; ++ i)
        {
            const int index = m_fired[i];
            m_v[index] = m_c[index];
            m_u[index] += m_d[index];
        }

        // update input current
        if (!m_fired.empty())
            for (int i = 0; i < m_N; ++ i)
            {
                double neuron_I = 0.0;
                for (int j = 0, n = m_fired.size(); j < n; ++ j)
                    neuron_I += m_s.at(i, m_fired[j]);
                m_I[i] += neuron_I;
            }
    }

    void network::update_potentials()
    {
        for (int i = 0; i < 2; ++ i)
            m_v += 0.5 * ((0.04 * m_v + 5.0) % m_v + 140.0 - m_u + m_I);
        m_u += (m_a % (m_b % m_v - m_u));
    }

} // namespace snn
