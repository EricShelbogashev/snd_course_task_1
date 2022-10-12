#include "Pair.h"

template<class T>
T Pair<T>::get_first() const {
    return a_;
}

template<class T>
T Pair<T>::get_second() const {
    return b_;
}

template<class T>
Pair<T>::Pair(T a, T b) : a_(a), b_(b) {}

template<class T>
Pair<T>::Pair(const Pair<T> &other) : a_(other.get_first()), b_(other.get_second()){}
