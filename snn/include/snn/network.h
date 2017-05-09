#ifndef __SNN_NETWORK_H__
#define __SNN_NETWORK_H__

#include <armadillo>
#include <random>
#include <vector>

namespace snn {

    class network
    {
    public:
        void generate_random_input();
        void find_fired_neurons();
        void reset_fired_neurons();
        void process_firings();
        void update_potentials();

    public:
        network(int N);

    private:
        void initialize_post_synaptic_connections();

    protected:
        std::default_random_engine m_random_engine;
        int m_N;
        int m_Ne;
        int m_Ni;
        int m_M;
        arma::vec m_a;
        arma::vec m_b;
        arma::vec m_c;
        arma::vec m_d;
        arma::mat m_s;
        arma::vec m_v;
        arma::vec m_u;
        arma::vec m_I;
        std::vector<int> m_fired;
        std::vector<std::vector<int>> m_post;
    };

} // namespace snn

#endif // __SNN_NETWORK_H__
