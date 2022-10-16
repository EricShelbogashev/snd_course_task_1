#ifndef LINKEDHASHSET_ENTRY_H
#define LINKEDHASHSET_ENTRY_H

#include <cstdio>
#include <iostream>
#include <list>
#include "Student.h"

template<class K>
class Entry {
public:

    explicit Entry(typename std::list<K *>::iterator iterator, K value);

    Entry(const Entry<K> &other);

    ~Entry() = default;

    bool operator==(const Entry<K> & other) const;

    bool operator!=(const Entry<K> & other) const;

    typename std::list<K *>::iterator iterator;
    K value;
};

template class Entry<Student>;

#endif //LINKEDHASHSET_ENTRY_H
