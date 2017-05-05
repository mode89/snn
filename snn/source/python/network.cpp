#include <snn/python/network.h>

#define NDARRAY_FROM_DATA(data) \
    np::from_data( \
        data.memptr(), \
        np::dtype::get_builtin<double>(), \
        p::make_tuple(data.n_rows, data.n_cols), \
        p::make_tuple(sizeof(double), sizeof(double) * data.n_rows), \
        m_numpy_memory_owner)

#define NDARRAY_MEMBER(member) \
    member(NDARRAY_FROM_DATA(snn::network::member))

namespace snn { namespace python {

    namespace p = boost::python;

    network::network(int N)
        : snn::network(N)
        , NDARRAY_MEMBER(m_a)
        , NDARRAY_MEMBER(m_b)
        , NDARRAY_MEMBER(m_c)
        , NDARRAY_MEMBER(m_d)
        , NDARRAY_MEMBER(m_s)
        , NDARRAY_MEMBER(m_I)
    {
    }

}} // namespace snn::python
