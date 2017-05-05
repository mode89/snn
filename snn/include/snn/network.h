#ifndef __SNN_NETWORK_H__
#define __SNN_NETWORK_H__

#include <armadillo>

namespace snn {

    class network
    {
    public:
        network(int N);

    protected:
        int m_N;
        int m_Ne;
        int m_Ni;
        arma::vec m_a;
        arma::vec m_b;
        arma::vec m_c;
        arma::vec m_d;
        arma::mat m_s;
        arma::vec m_I;
    };

} // namespace snn

#endif // __SNN_NETWORK_H__
