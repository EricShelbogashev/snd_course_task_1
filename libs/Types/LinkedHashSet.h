#ifndef LINKEDHASHMAP_LINKEDHASHSET_H
#define LINKEDHASHMAP_LINKEDHASHSET_H

#pragma once

#include <list>
#include <vector>
#include "Student.h"
#include "Pair.h"

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

    class iterator {
    public:
        explicit iterator(std::list<Pair<size_t>>::iterator it, LinkedHashSet * lhs);
//        explicit iterator(const std::list<Pair<size_t>>::iterator it, LinkedHashSet & lhs);
        element operator*();
        iterator & operator++();
        iterator operator++(int);
        iterator & operator--();
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;
    private:
        friend LinkedHashSet;
        std::list<Pair<size_t>>::iterator hist_iter_;
        LinkedHashSet *lhs;
    };

    iterator find(const element &e);
    iterator begin();
    iterator end();

private:
    friend class LoadPrivateDataGTest;

    static const size_t DEFAULT_CAPACITY_ = 8;
    static constexpr double OCCUPACITY_COEFFICIENT_ = 0.75;

    size_t elem_count_;
    size_t arr_occupancy_; // The count of baskets.
    size_t arr_capacity_;

    std::list<element> ** arr_;
    std::list<Pair<size_t>> history_;
    inline size_t get_hash_pos_(const element &e) const;

    void hashmap_resize_(size_t new_capacity); // with rehash
    void clear_();
    void deep_delete_arr_();
    void deep_copy_arr_(const LinkedHashSet &other);
};

#endif //LINKEDHASHMAP_LINKEDHASHSET_H
