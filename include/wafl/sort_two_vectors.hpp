#include <iterator>

namespace SortHelper {

template<typename OrderContainer, typename DataContainer>
struct ValueReference;

template<typename OrderContainer, typename DataContainer>
struct Value {
    using OrderT         = typename OrderContainer::value_type;
    using DataT          = typename DataContainer::value_type;
    using OrderIteratorT = typename OrderContainer::iterator;
    using DataIteratorT  = typename DataContainer::iterator;
    OrderT Order;
    DataT  Data;

    Value(OrderT order, DataT data)
        : Order(order), Data(data)
    {
    }

    operator OrderT() const { return Order; }

    Value(const ValueReference<OrderContainer, DataContainer>& rhs);

    bool operator<(const Value<OrderContainer, DataContainer>& rhs) const { return Order < rhs.Order; }
};

template<typename OrderContainer, typename DataContainer>
struct ValueReference {
    using OrderT = typename OrderContainer::value_type;
    // using DataT          = DataContainer::value_type;
    using OrderIteratorT = typename OrderContainer::iterator;
    using DataIteratorT  = typename DataContainer::iterator;
    OrderIteratorT Order;
    DataIteratorT  Data;

    ValueReference(OrderIteratorT Order, DataIteratorT Data)
        : Order{Order}, Data{Data}
    {}

    operator OrderT() const
    {
        return *Order;
    }

    ValueReference& operator=(const ValueReference<OrderContainer, DataContainer>& rhs)
    {
        *Order = *rhs.Order;
        *Data  = *rhs.Data;
        return *this;
    }

    ValueReference& operator=(const Value<OrderContainer, DataContainer>& rhs)
    {
        *Order = rhs.Order;
        *Data  = rhs.Data;
        return *this;
    }

    bool operator<(const ValueReference<OrderContainer, DataContainer>& rhs) const { return *Order < *rhs.Order; }
};

template<typename OrderContainer, typename DataContainer>
struct ValueIterator {
    using OrderT         = typename OrderContainer::value_type;
    using DataT          = typename DataContainer::value_type;
    using OrderIteratorT = typename OrderContainer::iterator;
    using DataIteratorT  = typename DataContainer::iterator;
    typedef Value<OrderContainer, DataContainer> value_type;
    // typedef Value<OrderT, DataT>*                                        pointer;
    typedef ValueReference<OrderContainer, DataContainer> reference;
    typedef std::ptrdiff_t                                difference_type;
    typedef std::random_access_iterator_tag               iterator_category;

    OrderIteratorT OrderIterator;
    DataIteratorT  DataIterator;

    ValueIterator(OrderIteratorT orderIterator, DataIteratorT dataIterator)
        : OrderIterator(orderIterator), DataIterator(dataIterator)
    {
    }

    std::ptrdiff_t operator-(const ValueIterator& rhs) const { return OrderIterator - rhs.OrderIterator; }
    ValueIterator  operator+(std::ptrdiff_t off) const { return ValueIterator(OrderIterator + off, DataIterator + off); }
    ValueIterator  operator-(std::ptrdiff_t off) const { return ValueIterator(OrderIterator - off, DataIterator - off); }

    ValueIterator& operator++()
    {
        ++OrderIterator;
        ++DataIterator;
        return *this;
    }

    ValueIterator& operator--()
    {
        --OrderIterator;
        --DataIterator;
        return *this;
    }

    ValueIterator                                 operator++(int) { return ValueIterator(OrderIterator++, DataIterator++); }
    ValueIterator                                 operator--(int) { return ValueIterator(OrderIterator--, DataIterator--); }
    Value<OrderContainer, DataContainer>          operator*() const { return Value<OrderContainer, DataContainer>(*OrderIterator, *DataIterator); }
    ValueReference<OrderContainer, DataContainer> operator[](difference_type n) const { return ValueReference<OrderContainer, DataContainer>(OrderIterator + n, DataIterator + n); }
    ValueReference<OrderContainer, DataContainer> operator*() { return ValueReference<OrderContainer, DataContainer>(OrderIterator, DataIterator); }
    bool                                          operator<(const ValueIterator& rhs) const { return OrderIterator < rhs.OrderIterator; }
    bool                                          operator==(const ValueIterator& rhs) const { return OrderIterator == rhs.OrderIterator; }
    bool                                          operator!=(const ValueIterator& rhs) const { return OrderIterator != rhs.OrderIterator; }
};

template<typename OrderContainer, typename DataContainer>
Value<OrderContainer, DataContainer>::Value(const ValueReference<OrderContainer, DataContainer>& rhs)
    : Order(*rhs.Order), Data(*rhs.Data)
{
}

template<typename OrderContainer, typename DataContainer>
bool operator<(const Value<OrderContainer, DataContainer>& lhs, const ValueReference<OrderContainer, DataContainer>& rhs)
{
    return lhs.Order < *rhs.Order;
}

template<typename OrderContainer, typename DataContainer>
bool operator<(const ValueReference<OrderContainer, DataContainer>& lhs, const Value<OrderContainer, DataContainer>& rhs)
{
    return *lhs.Order < rhs.Order;
}

template<typename OrderContainer, typename DataContainer>
void swap(ValueReference<OrderContainer, DataContainer> lhs, ValueReference<OrderContainer, DataContainer> rhs)
{
    std::swap(*lhs.Order, *rhs.Order);
    std::swap(*lhs.Data, *rhs.Data);
}
} // namespace SortHelper