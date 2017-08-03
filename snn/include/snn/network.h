#ifndef __SNN_NETWORK_H__
#define __SNN_NETWORK_H__

#include <armadillo>
#include <random>
#include <snn/circular_iterator.h>
#include <vector>

namespace snn {

    class network
    {
    public:
        void generate_random_input();
        void find_fired_neurons();
        void reset_fired_neurons();
        void process_firings();
        void update_weights();
        void update_potentials();

    public:
        network(int N);

    private:
        void initialize_synaptic_connections();
        void initialize_synaptic_weights();

    protected:
        std::default_random_engine m_random_engine;
        int m_N;
        int m_M;
        arma::fmat m_weight;
        arma::fvec m_v;
        arma::fvec m_I;
        std::vector<std::vector<int>> m_post;
        std::vector<std::vector<int>> m_pre;
        std::vector<int> m_fired;
        arma::fvec m_stdp;
        arma::fmat m_weightDelta;
        const float m_minWeight;
        const float m_maxWeight;
    };

} // namespace snn

#endif // __SNN_NETWORK_H__
