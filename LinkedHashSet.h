#ifndef TASK1_LINKEDHASHSET_H
#define TASK1_LINKEDHASHSET_H

#include <list>
#include <vector>
#include "Types/Student.h"

#define element Student

class LinkedHashSet {
public:
    /*class iterator {
    public:

        iterator();

        element operator*();

        const iterator operator++(int);

        iterator operator--();

        bool operator==(const iterator &other) const;

        bool operator!=(const iterator &other) const;
    private:
        int position_;
        std::list<element> current_list_;
    };*/

    LinkedHashSet();

    ~LinkedHashSet();

    LinkedHashSet(const LinkedHashSet &other);

    bool insert(const element &e);

    bool remove(const element &e);

    void swap(LinkedHashSet & other);

    size_t size() const;

    bool empty() const;

    bool contains(const element & e) const;

    LinkedHashSet &operator=(const LinkedHashSet &other);
    bool operator==(const LinkedHashSet & other) const;
    bool operator!=(const LinkedHashSet & other) const;

//    iterator find(const element & e) const;
//    iterator begin();
//    iterator end();

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


#endif