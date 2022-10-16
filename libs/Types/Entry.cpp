#include "Entry.h"

template<class K>
Entry<K>::Entry(typename std::list<K *>::iterator iterator, K value): iterator(iterator), value(value) {}

template<class K>
Entry<K>::Entry(const Entry<K> &other): iterator(other.iterator), value(other.value) {}

template<class K>
bool Entry<K>::operator==(const Entry<K> &other) const {
    return iterator == other.iterator && value == other.value;
}

template<class K>
bool Entry<K>::operator!=(const Entry<K> &other) const {
    return !operator==(other);
}

