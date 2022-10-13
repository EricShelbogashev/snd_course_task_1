#ifndef LINKEDHASHSET_PAIR_H
#define LINKEDHASHSET_PAIR_H

#include <cstdio>
#include "Student.h"

template<class K, class V>
class Pair {
public:
    explicit Pair(K a, V b);

    Pair(const Pair<K, V> &other);

    ~Pair() = default;

    K get_first() const ;

    V get_second() const;

    bool operator==(const Pair<K> & other) const;

    bool operator!=(const Pair<V> & other) const;

private:
    K a_;
    V b_;
};

template class Pair<size_t, Student>;

#endif //LINKEDHASHSET_PAIR_H
