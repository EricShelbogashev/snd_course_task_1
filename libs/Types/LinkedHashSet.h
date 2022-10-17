#ifndef LINKEDHASHMAP_LINKEDHASHSET_H
#define LINKEDHASHMAP_LINKEDHASHSET_H

#pragma once

#include <list>
#include <vector>
#include "Student.h"

typedef Student element;

class LinkedHashSet {
public:

    LinkedHashSet();
    explicit LinkedHashSet(size_t capacity);

    ~LinkedHashSet();
    LinkedHashSet(const LinkedHashSet &other);

    bool insert(const element &e);
    bool remove(const element &e);

    void swap(LinkedHashSet &other);
    size_t size() const;
    bool empty() const;
    bool contains(const element &e) const;
    // New method; not declared.
    LinkedHashSet &clear();

    LinkedHashSet &operator=(const LinkedHashSet &other);
    bool operator==(const LinkedHashSet &other);
    bool operator!=(const LinkedHashSet &other);

    /*class iterator {
    public:
        explicit iterator(std::list<element>::iterator it, LinkedHashSet * lhs);
        element operator*();
        iterator & operator++();
        iterator operator++(int);
        iterator & operator--();
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;
    private:
        friend LinkedHashSet;
        std::list<element *>::iterator hist_iter_;
        LinkedHashSet *lhs;
    };*/

    std::list<element>::iterator find(const element &e);
    std::list<element>::iterator begin();
    std::list<element>::iterator end();

private:
    friend class LoadPrivateDataGTest;
    template<class T> class Entry {
    public:
        explicit Entry(T &value, typename std::list<T>::iterator iterator);
        Entry(const Entry<T> &other);
        ~Entry() = default;
        bool operator==(const Entry<T> & other) const;
        bool operator!=(const Entry<T> & other) const;

    private:
        friend LinkedHashSet;
        typename std::list<T>::iterator iterator_;
        T &value_;
    };
    static const size_t DEFAULT_CAPACITY_ = 8;
    static constexpr double OCCUPACITY_COEFFICIENT_ = 0.75;

    size_t elem_count_;
    size_t arr_occupancy_; // The count of baskets.
    size_t arr_capacity_;

    std::list<Entry<element>> ** arr_;
    std::list<element> *history_;
    inline size_t get_hash_pos_(const element &e) const;

    void hashset_resize_(size_t new_capacity); // with rehash
    void clear_();
    std::list<Entry<element>>::iterator list_find_(std::list<Entry<element>> &list, const element &e);
    std::list<Entry<element>>::iterator arr_find_(const element &e);
    void deep_delete_arr_();
    void deep_copy_arr_(const LinkedHashSet &other);
};



#endif //LINKEDHASHMAP_LINKEDHASHSET_H
