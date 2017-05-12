#include <algorithm>
#include <snn/network.h>

namespace snn {

    network::network(int N, int D)
        : m_N(N)
        , m_Ne(N * 0.8)
        , m_Ni(m_N - m_Ne)
        , m_M(m_N * 0.1)
        , m_D(D)
        , m_a(N)
        , m_b(N)
        , m_c(N)
        , m_d(N)
        , m_s(N, N)
        , m_v(N)
        , m_u(N)
        , m_I(N)
        , m_post()
        , m_delays()
        , m_firings(D)
        , m_fired(m_firings.begin(), m_firings)
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

        initialize_post_synaptic_connections();
        initialize_delays();
        initialize_synaptic_weights();

        // initialize vectors v and u
        m_v = m_c;
        m_u = m_b % m_v;

        // reserved memory for firings
        for (auto buf: m_firings)
            buf.reserve(m_N);
    }

    void network::initialize_post_synaptic_connections()
    {
        m_post.resize(m_N);

        std::vector<int> all_indices(m_N);
        std::iota(all_indices.begin(), all_indices.end(), 0);

        for (int i = 0; i < m_Ne; ++ i)
        {
            std::shuffle(all_indices.begin(), all_indices.end(),
                m_random_engine);
            std::copy_n(all_indices.begin(), m_M,
                std::back_inserter(m_post[i]));
        }

        std::vector<int> exitatory_indices(m_Ne);
        std::iota(exitatory_indices.begin(), exitatory_indices.end(), 0);

        for (int i = m_Ne; i < m_N; ++ i)
        {
            std::shuffle(exitatory_indices.begin(),
                exitatory_indices.end(), m_random_engine);
            std::copy_n(exitatory_indices.begin(), m_M,
                std::back_inserter(m_post[i]));
        }
    }

    void network::initialize_delays()
    {
        m_delays.resize(m_N);
        std::uniform_int_distribution<int> delay_dist(0, m_D - 1);

        for (int i = 0; i < m_Ne; ++ i)
        {
            m_delays[i].resize(m_D);
            for (int j = 0; j < m_M; ++ j)
            {
                const int delay = delay_dist(m_random_engine);
                const int post_neuron = m_post[i][j];
                m_delays[i][delay].push_back(post_neuron);
            }
        }

        for (int i = m_Ne; i < m_N; ++ i)
        {
            m_delays[i].resize(m_D);
            for (int j = 0; j < m_M; ++ j)
            {
                const int post_neuron = m_post[i][j];
                m_delays[i][0].push_back(post_neuron);
            }
        }
    }

    void network::initialize_synaptic_weights()
    {
        m_s = arma::zeros(m_N, m_N);

        for (int i = 0; i < m_Ne; ++ i)
        {
            for (int j = 0; j < m_M; ++ j)
            {
                const int neuron = m_post[i][j];
                m_s(i, neuron) = 6.0;
            }
        }

        for (int i = m_Ne; i < m_N; ++ i)
        {
            for (int j = 0; j < m_M; ++ j)
            {
                const int neuron = m_post[i][j];
                m_s(i, neuron) = -5.0;
            }
        }
    }

    void network::generate_random_input()
    {
        std::uniform_real_distribution<double> dist(-6.5, 6.5);
        for (int i = 0; i < m_N; ++ i)
            m_I[i] = dist(m_random_engine);
    }

    void network::find_fired_neurons()
    {
        // prepare the next firings' buffer
        ++ m_fired;
        m_fired->clear();

        // detect fired neurons
        for (int i = 0; i < m_N; ++ i)
            if (m_v[i] >= 30.0)
                m_fired->push_back(i);
    }

    void network::reset_fired_neurons()
    {
        // reset potential of fired neurons
        const auto & fired = *m_fired;
        for (int i, n = fired.size(); i < n; ++ i)
        {
            const int index = fired[i];
            m_v[index] = m_c[index];
            m_u[index] += m_d[index];
        }
    }

    void network::process_firings()
    {
        find_fired_neurons();
        reset_fired_neurons();

        // update input current
        const auto & fired = *m_fired;
        if (!fired.empty())
            for (int i = 0; i < m_N; ++ i)
            {
                double neuron_I = 0.0;
                for (int j = 0, n = fired.size(); j < n; ++ j)
                    neuron_I += m_s.at(i, fired[j]);
                m_I[i] += neuron_I;
            }
    }

    void network::deliver_delayed_spikes()
    {
        auto fired_iter = m_fired;
        for (int time = 0; time < m_D; ++ time, -- fired_iter)
        {
            const auto & fired = *fired_iter;
            for (int fired_neuron: fired)
            {
                const auto & fired_post = m_delays[fired_neuron][time];
                for (int post_neuron: fired_post)
                    m_I[post_neuron] += m_s.at(fired_neuron, post_neuron);
            }
        }
    }

    void network::update_potentials()
    {
        for (int i = 0; i < 2; ++ i)
            m_v += 0.5 * ((0.04 * m_v + 5.0) % m_v + 140.0 - m_u + m_I);
        m_u += (m_a % (m_b % m_v - m_u));
    }

} // namespace snn
