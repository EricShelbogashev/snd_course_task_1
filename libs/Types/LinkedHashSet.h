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
    bool insert(const T &e);
    bool remove(const T &e);
    void swap(LinkedHashSet &other);
    size_t size() const;
    bool empty() const;
    bool contains(const T &e) const;
    LinkedHashSet &clear();
    LinkedHashSet &operator=(const LinkedHashSet &other);
    bool operator==(const LinkedHashSet &other);
    bool operator!=(const LinkedHashSet &other);
    typename std::list<T>::iterator find(const T &e);
    typename std::list<T>::iterator begin();
    typename std::list<T>::iterator end();

private:
    // CR: <class V>
    template<class K>
    class Entry {
    public:
        explicit Entry(K &value, typename std::list<K>::iterator &iterator);
        Entry(const Entry<K> &other);
        bool operator==(const Entry<K> &other) const;
        bool operator!=(const Entry<K> &other) const;
    private:
        friend LinkedHashSet;
        typename std::list<K>::iterator iterator_;
        K &value_;
    };

    static const size_t DEFAULT_CAPACITY_ = 8;
    constexpr static const double CAPACITY_COEFF_ = 0.85;
    size_t arr_capacity_;
    /* Container of buckets lists. */
    std::list<Entry<T>> **arr_;
    /* Container of elements. */
    std::list<T> history_;
    inline size_t get_hash_pos_(const T &e, size_t capacity) const;
    void init_hashset(size_t capacity);
    inline void delete_arr_();
    void deep_delete_sub_arr_();
};

#include "LinkedHashSet.hpp"


#endif //LINKEDHASHMAP_LINKEDHASHSET_H
