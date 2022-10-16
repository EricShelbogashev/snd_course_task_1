#include "Pair.h"

template<class K, class V>
bool Pair<K, V>::operator==(const Pair<K, V> &other) const {
    return a_ == other.a_ && b_ == other.b_;
}

template<class K, class V>
bool Pair<K, V>::operator!=(const Pair<K, V> &other) const {
    return !operator==(other);
}

template<class K, class V>
K Pair<K, V>::get_first() const {
    return a_;
}

template<class K, class V>
V Pair<K, V>::get_second() const {
    return b_;
}

template<class K, class V>
Pair<K, V>::Pair(const Pair<K, V> &other): a_(other.get_first()), b_(other.get_second()) {

}

template<class K, class V>
Pair<K, V>::Pair(K a, V b): a_(a), b_(b) {}
