#include <algorithm>
#include <snn/connections.h>
#include <snn/group.h>
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
    }

    std::shared_ptr<group> network::add_group(unsigned size)
    {
        auto newGroup = std::make_shared<group>(size, m_random_engine);
        m_groups.push_back(newGroup);
        return newGroup;
    }

    std::shared_ptr<connections> network::connect(
        const std::shared_ptr<group> & preGroup,
        const std::shared_ptr<group> & postGroup,
        const float connectivity,
        const float minWeight,
        const float maxWeight)
    {
        auto newConn = std::make_shared<connections>(
            preGroup,
            postGroup,
            connectivity,
            minWeight,
            maxWeight,
            m_random_engine);
        m_connections.push_back(newConn);
        return newConn;
    }

    void network::generate_random_input()
    {
        m_I.zeros();
        std::uniform_int_distribution<int> dist(0, m_N);
        m_I[dist(m_random_engine)] = 1.0;
    }

    void network::find_fired_neurons()
    {
        for (auto & grp: m_groups)
            grp->find_fired_neurons();
    }

    void network::reset_fired_neurons()
    {
        for (auto & grp: m_groups)
            grp->reset_fired_neurons();
    }

    void network::deliver_spikes()
    {
        for (auto & conns: m_connections)
            conns->deliver_spikes();
    }

    void network::process_firings()
    {
        find_fired_neurons();
        reset_fired_neurons();
        deliver_spikes();
    }

    void network::update_weights()
    {
        for (auto grp: m_groups)
            grp->update_stdp();

        for (auto conns: m_connections)
            conns->update_weights();
    }

    void network::update_potentials()
    {
        for (auto grp: m_groups)
            grp->update_potentials();
    }

} // namespace snn
