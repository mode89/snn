#ifndef __SNN_CONNECTIONS_H__
#define __SNN_CONNECTIONS_H__

#include <memory>
#include <random>
#include <snn/types_fwd.h>
#include <vector>

namespace snn {

    class connections
    {
    public:
        void deliver_spikes();
        void update_weights();

    public:
        connections(
            const std::shared_ptr<group> & preGroup,
            const std::shared_ptr<group> & postGroup,
            const float connectivity,
            const float minWeight,
            const float maxWeight,
            std::default_random_engine &);

    public:
        const std::shared_ptr<group> m_preGroup;
        const std::shared_ptr<group> m_postGroup;
        const float m_minWeight;
        const float m_maxWeight;
        std::default_random_engine & m_randomEngine;
        std::vector<std::vector<int>> m_postNeuron;
        std::vector<std::vector<int>> m_preNeuron;
        std::vector<std::vector<float>> m_weight;
        std::vector<std::vector<float>> m_weightDelta;
    };

} // namespace snn

#endif // __SNN_CONNECTIONS_H__
