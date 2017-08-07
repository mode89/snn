#include <algorithm>
#include <snn/connections.h>
#include <snn/group.h>

namespace snn {

    connections::connections(
        const std::shared_ptr<group> & preGroup,
        const std::shared_ptr<group> & postGroup,
        const float connectivity,
        const float minWeight,
        const float maxWeight,
        std::default_random_engine & randomEngine)
        : m_preGroup(preGroup)
        , m_postGroup(postGroup)
        , m_minWeight(minWeight)
        , m_maxWeight(maxWeight)
        , m_randomEngine(randomEngine)
        , m_preNeuron(postGroup->size())
        , m_postNeuron(preGroup->size())
        , m_weight(preGroup->size())
        , m_weightDelta(preGroup->size())
    {
        std::vector<int> allPostIndices(m_postGroup->size());
        std::iota(allPostIndices.begin(), allPostIndices.end(), 0);

        // generate post-synaptic connections
        const unsigned preNeuronNum = m_preGroup->size();
        for (unsigned preNeuron = 0; preNeuron < preNeuronNum; ++ preNeuron)
        {
            std::shuffle(
                allPostIndices.begin(),
                allPostIndices.end(),
                m_randomEngine);

            const unsigned postNeuronNum =
                m_postGroup->size() * connectivity;
            m_postNeuron[preNeuron].reserve(postNeuronNum);
            std::copy_n(
                allPostIndices.begin(),
                postNeuronNum,
                std::back_inserter(m_postNeuron[preNeuron]));
        }

        // track pre-synaptic neurons
        for (unsigned preNeuron = 0; preNeuron < preNeuronNum; ++ preNeuron)
        {
            for (const unsigned postNeuron: m_postNeuron[preNeuron])
                m_preNeuron[postNeuron].push_back(preNeuron);
        }

        // initialize random weights
        std::uniform_real_distribution<>
            distWeight(m_minWeight, m_maxWeight);
        for (unsigned preNeuron = 0; preNeuron < preNeuronNum; ++ preNeuron)
        {
            const unsigned postNeuronNum = m_postNeuron[preNeuron].size();
            m_weight[preNeuron].resize(postNeuronNum);
            for (const unsigned postNeuron: m_postNeuron[preNeuron])
                m_weight[preNeuron][postNeuron] =
                    distWeight(m_randomEngine);
        }

        // initialize weight changes
        for (unsigned preNeuron = 0; preNeuron < preNeuronNum; ++ preNeuron)
        {
            const unsigned postNeuronNum = m_postNeuron[preNeuron].size();
            m_weightDelta[preNeuron].resize(postNeuronNum);
            for (const unsigned postNeuron: m_postNeuron[preNeuron])
                m_weightDelta[preNeuron][postNeuron] = 0.0f;
        }
    }

    void connections::deliver_spikes()
    {
        const auto & firedPreNeurons = m_preGroup->get_fired_neurons();
        auto & postCurrent = m_postGroup->current();
        for (const unsigned preNeuron: firedPreNeurons)
            for (const unsigned postNeuron: m_postNeuron[preNeuron])
                postCurrent[postNeuron] += m_weight[preNeuron][postNeuron];
    }

    void connections::update_weights()
    {
        // long-term depression
        const auto & firedPreNeurons = m_preGroup->get_fired_neurons();
        const auto & postSTDP = m_postGroup->get_stdp();
        for (const unsigned preNeuron: firedPreNeurons)
            for (const unsigned postNeuron: m_postNeuron[preNeuron])
                m_weightDelta[preNeuron][postNeuron] -=
                    postSTDP[postNeuron];

        // long-term potentiation
        const auto & firedPostNeurons = m_postGroup->get_fired_neurons();
        const auto & preSTDP = m_preGroup->get_stdp();
        for (const unsigned postNeuron: firedPostNeurons)
            for (const unsigned preNeuron: m_preNeuron[preNeuron])
                m_weightDelta[preNeuron][postNeuron] += preSTDP[preNeuron];

        const unsigned preNeuronNum = m_preGroup->size();
        const float weightDeltaTau = 1000.0f;
        const float weightDeltaUpdate = 1.0f - 1.0f / weightDeltaTau;
        for (unsigned preNeuron = 0; preNeuron < preNeuronNum; ++ preNeuron)
        {
            for (const unsigned postNeuron: m_postNeuron[preNeuron])
            {
                auto & weight = m_weight[preNeuron][postNeuron];
                auto & delta = m_weightDelta[preNeuron][postNeuron];

                // decay weight change
                delta *= weightDeltaUpdate;

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

} // namespace snn
