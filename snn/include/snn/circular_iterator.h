#ifndef __SNN_CIRCULAR_ITERATOR_H__
#define __SNN_CIRCULAR_ITERATOR_H__

namespace snn {

    template <class Container>
    class circular_iterator
    {
    public:
        using self_type = circular_iterator<Container>;
        using container_type = Container;
        using iterator_type = typename container_type::iterator;

        circular_iterator(const iterator_type & it, container_type & c)
            : m_container(c)
            , m_iterator(it)
        {
        }

        circular_iterator(const self_type &) = default;

        circular_iterator & operator=(const self_type & other)
        {
            m_container = other.m_container;
            m_iterator = other.m_iterator;
            return *this;
        }

        self_type & operator++(void)
        {
            ++ m_iterator;
            if (m_iterator == m_container.end())
                m_iterator = m_container.begin();
            return *this;
        }

        self_type & operator--(void)
        {
            if (m_iterator == m_container.begin())
                m_iterator = m_container.end();
            -- m_iterator;
            return *this;
        }

        bool operator==(const self_type & other)
        {
            return m_iterator == other.m_iterator;
        }

        bool operator!=(const self_type & other)
        {
            return m_iterator != other.m_iterator;
        }

        typename iterator_type::pointer operator->()
        {
            return m_iterator.operator->();
        }

        typename iterator_type::reference operator*()
        {
            return m_iterator.operator*();
        }

        self_type prev() const
        {
            auto it = m_iterator;
            if (it == m_container.begin())
                it = m_container.end();
            return self_type(std::prev(it), m_container);
        }

    public:
        container_type & m_container;
        iterator_type m_iterator;
    };

} // namespace snn

#endif // __SNN_CIRCULAR_ITERATOR_H__
