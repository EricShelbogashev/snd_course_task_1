#ifndef TEST11_LINKEDHASHSET_H
#define TEST11_LINKEDHASHSET_H

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

    LinkedHashSet &operator=(const LinkedHashSet &other);
    bool operator==(const LinkedHashSet &other) const;
    bool operator!=(const LinkedHashSet &other) const;

    class iterator {
    public:
        element operator*();
        iterator operator++(int);
        iterator operator--();
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;
    };

    iterator find(const element &e) const;
    iterator begin() const;
    iterator end() const;

private:
    static const size_t DEFAULT_CAPACITY_ = 8;
    static constexpr double OCCUPACITY_COEFFICIENT_ = 0.75;

    size_t elem_count_;
    size_t arr_occupancy_; // The count of baskets.
    size_t arr_capacity_;

    std::list<element> ** arr_;

    inline size_t get_hash_pos_(const element &e) const;

    // void rehash_();
    // void resize_(size_t new_capacity);
    void hashmap_resize_(size_t new_capacity); // with rehash
    void clear_();
    void deep_delete_arr_();
    void deep_copy_arr_(const LinkedHashSet &other);
};



#endif //TEST11_LINKEDHASHSET_H
