#ifndef LINKEDHASHSET_PAIR_H
#define LINKEDHASHSET_PAIR_H

#include <cstdio>

template<class T>
class Pair {
public:
    explicit Pair(T a, T b);

    Pair(const Pair<T> &other);

    ~Pair() = default;

    T get_first() const ;

    T get_second() const;

    bool operator==(const Pair<T> & other) const;

    bool operator!=(const Pair<T> & other) const;

private:
    T a_;
    T b_;
};

template class Pair<size_t>;

#endif //LINKEDHASHSET_PAIR_H
