#ifndef TASK1_LINKEDHASHSET_H
#define TASK1_LINKEDHASHSET_H

#include <list>
#include <vector>
#include "Types/Student.h"


#define element Student


class Iterator;

class LinkedHashSet {
public:
    friend class Iterator;

    LinkedHashSet();
    ~LinkedHashSet();
    LinkedHashSet(const LinkedHashSet &other);

    bool insert(const element &e);
    bool remove(const element &e);

    void swap(LinkedHashSet &other);
    size_t size() const;
    bool empty() const;
    bool contains(const element &e) const;

    LinkedHashSet &operator=(const LinkedHashSet &other);
    bool operator==(const LinkedHashSet &other) const;
    bool operator!=(const LinkedHashSet &other) const;

//    Iterator find(const element &e) const;
//    Iterator begin();
//    Iterator end();

private:
    int size_;
    int capacity_;
    const float occupancy_ = 0.75;
    std::vector<std::list<element>> arr_;

    long long getHashPos_(const element *e) const;
    void rehash_(int old_capacity);
    void resize_(int new_capacity);
    void clear_();

};

/*

class Iterator {
public:
    Iterator(LinkedHashSet &lHS);
    element operator*(LinkedHashSet &lHS);
    Iterator operator++(LinkedHashSet &lHS, int);
    Iterator operator--(LinkedHashSet &lHS);
    bool operator==(const Iterator &other) const;
    bool operator!=(const Iterator &other) const;

private:
    int size_position_;
    int hash_position_;
    std::list<element>::iterator curr_iter_;
};
*/


#endif