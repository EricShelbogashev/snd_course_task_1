#ifndef LINKEDHASHMAP_LINKEDHASHSET_H
#define LINKEDHASHMAP_LINKEDHASHSET_H

#include <list>
#include <vector>
#include <algorithm>

template<typename T, typename Hasher=std::hash<T>>
class LinkedHashSet {
public:
    // CR: move comments here
    explicit LinkedHashSet(size_t capacity = DEFAULT_CAPACITY_);

    ~LinkedHashSet();

    LinkedHashSet(const LinkedHashSet &other);

    /* Put element of T type in hashset. */
    bool insert(const T &e);

    /* Remove element from hashset. */
    bool remove(const T &e);

    void swap(LinkedHashSet &other);

    size_t size() const;

    bool empty() const;

    bool contains(const T &e) const;

    /* Restore hashset to original form (including size and capacity. */
    LinkedHashSet &clear();

    LinkedHashSet &operator=(LinkedHashSet other);

    bool operator==(const LinkedHashSet &other);

    bool operator!=(const LinkedHashSet &other);

    typename std::list<T>::iterator find(const T &e);

    typename std::list<T>::iterator begin();

    typename std::list<T>::iterator end();

private:
    class Entry {
    public:
        explicit Entry(T &value, typename std::list<T>::iterator &iterator);

        Entry(const Entry &other);

        bool operator==(const Entry &other) const;

        bool operator!=(const Entry &other) const;

    private:
        friend LinkedHashSet;
        typename std::list<T>::iterator iterator_;
        T &value_;
    };

    static const size_t DEFAULT_CAPACITY_ = 8;
    constexpr static const double CAPACITY_COEFF_ = 0.85;
    size_t arr_capacity_;
    /* Container of buckets lists. */
    std::list<Entry> **arr_;
    /* history_ is a storage of elements;
     * arr_ buckets contains an Entry with an iterator to the element and its pointer for O(1) fast access. */
    std::list<T> history_;
    Hasher hasher_;
    /* Returns index of bucket list in arr_;
     * Uses special Hasher class with () operator overridden to get hash and convert it to position in bucket array. */
    inline size_t get_hash_pos_(const T &e, size_t capacity) const;

    inline void delete_arr_();

    /* Restore arr_ from history_ with new capacity;
     * Overrides arr_ without delete[] for previous arr_; */
    void resize_(size_t new_capacity);

    inline typename std::list<Entry>::iterator is_in_list_(std::list<Entry> *list, const T &e) const;

    inline std::list<Entry> &get_list(std::list<Entry> **arr, size_t pos);

    void insert_(std::list<Entry> **arr, const T &e, typename std::list<T>::iterator &it, size_t capacity);
};

#include "LinkedHashSet.hpp"


#endif //LINKEDHASHMAP_LINKEDHASHSET_H
