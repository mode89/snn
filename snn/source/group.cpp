#include <snn/group.h>

namespace snn {

    group::group(unsigned size, std::default_random_engine & randomEngine)
        : m_size(size)
        , m_randomEngine(randomEngine)
        , m_voltage(size, 0.0f)
        , m_current(size, 0.0f)
        , m_fired(0)
    {
        m_fired.reserve(size);
    }

    void group::generate_random_input()
    {
        for (auto & current: m_current)
            current = 0.0f;

        std::uniform_int_distribution<int> dist(0, m_size);
        m_current[dist(m_randomEngine)] = 1.0f;
    }

    void group::find_fired_neurons()
    {
        m_fired.clear();
        for (int i = 0; i < m_size; ++ i)
            if (m_voltage[i] > 1.0)
                m_fired.push_back(i);
    }

    void group::reset_fired_neurons()
    {
        for (const int i: m_fired)
            m_voltage[i] = 0.0f;
    }

    void group::update_stdp()
    {
        const float tau = 20.0f;
        const float advance = 1.0f - 1.0f / tau;
        for (auto & stdp: m_stdp)
            stdp *= advance;

        for (auto neuron: m_fired)
            m_stdp[neuron] = 0.1f;
    }

    void group::update_potentials()
    {
        for (unsigned i = 0; i < m_size; ++ i)
            m_voltage[i] += m_current[i];
    }

} // namespace snn
