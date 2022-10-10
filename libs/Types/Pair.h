#ifndef LINKEDHASHSET_PAIR_H
#define LINKEDHASHSET_PAIR_H

#include <cstdio>

template<class T>
class Pair {
public:
    explicit Pair(T a, T b);
    Pair(Pair &other);
    ~Pair() = default;

    T get_first();

    T get_second();

private:
    T a_;
    T b_;
};

template class Pair<size_t>;

#endif //LINKEDHASHSET_PAIR_H
