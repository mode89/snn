#include <algorithm>
#include <snn/network.h>

namespace snn {

    network::network(int N)
        : m_N(N)
        , m_M(m_N * 0.1)
        , m_weight(arma::zeros<arma::fmat>(N, N))
        , m_v(arma::zeros<arma::fvec>(N))
        , m_I(arma::zeros<arma::fvec>(N))
        , m_post(N)
        , m_pre(N)
        , m_fired()
        , m_random_engine(0)
        , m_stdp(arma::zeros<arma::fvec>(N))
        , m_weightDelta(arma::zeros<arma::fmat>(N, N))
        , m_minWeight(-100.0f / m_N)
        , m_maxWeight(100.0f / m_N)
    {
        initialize_synaptic_connections();
        initialize_synaptic_weights();

        m_fired.reserve(m_N);
    }

    void network::initialize_synaptic_connections()
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

        for (int i = 0; i < m_N; ++ i)
            for (auto neuron: m_post[i])
                m_pre[neuron].push_back(i);
    }

    void network::initialize_synaptic_weights()
    {
        std::uniform_real_distribution<>
            dist_weight(m_minWeight, m_maxWeight);

        for (int i = 0; i < m_N; ++ i)
        {
            for (int j = 0; j < m_M; ++ j)
            {
                const int neuron = m_post[i][j];
                m_weight(i, neuron) = dist_weight(m_random_engine);
            }
        }
    }

    void network::generate_random_input()
    {
        m_I.zeros();
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
                    neuron_I += m_weight.at(i, m_fired[j]);
                m_I[i] += neuron_I;
            }
    }

    void network::update_weights()
    {
        // update STDP
        m_stdp *= (1.0f - 1.0f / 20);
        for (auto neuron: m_fired)
            m_stdp[neuron] = 0.1f;

        // update weight change
        for (auto neuron: m_fired)
        {
            for (auto pre_neuron: m_pre[neuron])
                m_weightDelta.at(pre_neuron, neuron) +=
                    m_stdp[pre_neuron];

            for (auto post_neuron: m_post[neuron])
                m_weightDelta.at(neuron, post_neuron) -=
                    m_stdp[post_neuron];
        }

        // update weights
        for (int preN = 0; preN < m_N; ++ preN)
        {
            for (auto postN: m_post[preN])
            {
                auto & weight = m_weight.at(preN, postN);
                auto & delta = m_weightDelta.at(preN, postN);

                // decay weight change
                delta *= (1.0f - 1.0f / 1000.0f);

                // update and clamp wegith
                const float newWeight = weight + delta;
                if (newWeight < m_minWeight)
                    weight = m_minWeight;
                else if (newWeight > m_maxWeight)
                    weight = m_maxWeight;
                else
                    weight = newWeight;
            }
        }
    }

    void network::update_potentials()
    {
        m_v += m_I;
    }

} // namespace snn
