#include <algorithm>
#include <snn/network.h>

namespace snn {

    network::network(int N)
        : m_N(N)
        , m_M(m_N * 0.1)
        , m_s(arma::zeros(N, N))
        , m_v(arma::zeros(N))
        , m_I(arma::zeros(N))
        , m_post(N)
        , m_fired()
        , m_random_engine(0)
    {
        initialize_post_synaptic_connections();
        initialize_synaptic_weights();

        m_fired.reserve(m_N);
    }

    void network::initialize_post_synaptic_connections()
    {
        std::vector<int> all_indices(m_N);
        std::iota(all_indices.begin(), all_indices.end(), 0);

        for (int i = 0; i < m_N; ++ i)
        {
            std::shuffle(all_indices.begin(), all_indices.end(),
                m_random_engine);
            std::copy_n(all_indices.begin(), m_M,
                std::back_inserter(m_post[i]));
        }
    }

    void network::initialize_synaptic_weights()
    {
        std::uniform_real_distribution<> dist_s(-100.0 / m_N, 100.0 / m_N);
        for (int i = 0; i < m_N; ++ i)
        {
            for (int j = 0; j < m_M; ++ j)
            {
                const int neuron = m_post[i][j];
                m_s(i, neuron) = dist_s(m_random_engine);
            }
        }
    }

    void network::generate_random_input()
    {
        m_I = arma::zeros(m_N);
        std::uniform_int_distribution<int> dist(0, m_N);
        m_I[dist(m_random_engine)] = 1.0;
    }

    void network::find_fired_neurons()
    {
        m_fired.clear();

        // detect fired neurons
        for (int i = 0; i < m_N; ++ i)
        {
            if (m_v[i] >= 1.0)
                m_fired.push_back(i);
            if (m_v[i] < 0.0)
                m_v[i] = 0.0;
        }
    }

    void network::reset_fired_neurons()
    {
        // reset potential of fired neurons
        for (int i, n = m_fired.size(); i < n; ++ i)
        {
            const int index = m_fired[i];
            m_v[index] = 0.0;
        }
    }

    void network::process_firings()
    {
        find_fired_neurons();
        reset_fired_neurons();

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
        m_v += m_I;
    }

} // namespace snn
