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
        void deliver_delayed_spikes();
        void update_potentials();

    public:
        network(int N, int D);

    private:
        void initialize_post_synaptic_connections();
        void initialize_delays();
        void initialize_synaptic_weights();

    protected:
        typedef std::vector<std::vector<int>> firings_type;
        typedef circular_iterator<firings_type> firings_iterator;

        std::default_random_engine m_random_engine;
        int m_N;
        int m_Ne;
        int m_Ni;
        int m_M;
        int m_D;
        arma::vec m_a;
        arma::vec m_b;
        arma::vec m_c;
        arma::vec m_d;
        arma::mat m_s;
        arma::vec m_v;
        arma::vec m_u;
        arma::vec m_I;
        std::vector<std::vector<int>> m_post;
        std::vector<std::vector<std::vector<int>>> m_delays;
        firings_type m_firings;
        firings_iterator m_fired;
    };

} // namespace snn

#endif // __SNN_NETWORK_H__
