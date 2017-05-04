#ifndef __SNN_NETWORK_H__
#define __SNN_NETWORK_H__

namespace snn {

    class network
    {
    public:
        network(int N);

    private:
        int m_N;
        int m_Ne;
        int m_Ni;
    };

} // namespace snn

#endif // __SNN_NETWORK_H__
