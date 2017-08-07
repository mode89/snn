#ifndef __SNN_GROUP_H__
#define __SNN_GROUP_H__

#include <vector>
#include <random>
#include <snn/types_fwd.h>

namespace snn {

    class group
    {
    public:
        void generate_random_input();
        void find_fired_neurons();
        void reset_fired_neurons();
        void update_stdp();
        void update_potentials();

        unsigned size() const
            { return m_size; }

        std::vector<float> & current()
            { return m_current; }

        const std::vector<unsigned> & get_fired_neurons() const
            { return m_fired; }

        const std::vector<float> & get_stdp() const
            { return m_stdp; }

    public:
        group(unsigned size, std::default_random_engine &);

    private:
        unsigned m_size;
        std::default_random_engine & m_randomEngine;
        std::vector<float> m_voltage;
        std::vector<float> m_current;
        std::vector<unsigned> m_fired;
        std::vector<float> m_stdp;
    };

} // namespace snn

#endif // __SNN_GROUP_H__
