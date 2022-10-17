#ifndef LINKEDHASHMAP_LINKEDHASHSET_H
#define LINKEDHASHMAP_LINKEDHASHSET_H

#include <list>
#include <vector>
#include "Student.h"
#include "Path.h"

template<class T>
class LinkedHashSet {
public:

    LinkedHashSet();

    explicit LinkedHashSet(size_t capacity);

    ~LinkedHashSet();

    LinkedHashSet(const LinkedHashSet &other);

    bool insert(const T &e);

    bool remove(const T &e);

    void swap(LinkedHashSet &other);

    size_t size() const;

    bool empty() const;

    bool contains(const T &e) const;

    // New method; not declared.
    LinkedHashSet &clear();

    LinkedHashSet &operator=(const LinkedHashSet &other);

    bool operator==(const LinkedHashSet &other);

    bool operator!=(const LinkedHashSet &other);

    typename std::list<T>::iterator find(const T &e);

    typename std::list<T>::iterator begin();

    typename std::list<T>::iterator end();

private:
    friend class LoadPrivateDataGTest;

    template<class K>
    class Entry {
    public:
        explicit Entry(K &value, typename std::list<K>::iterator iterator);

        Entry(const Entry<K> &other);

        ~Entry() = default;

        bool operator==(const Entry<K> &other) const;

        bool operator!=(const Entry<K> &other) const;

    private:
        friend LinkedHashSet;
        typename std::list<K>::iterator iterator_;
        K &value_;
    };

    static const size_t DEFAULT_CAPACITY_ = 8;
    static constexpr double OCCUPACITY_COEFFICIENT_ = 0.75;

    size_t elem_count_;
    size_t arr_occupancy_; // The count of baskets.
    size_t arr_capacity_;

    std::list<Entry<T>> **arr_;
    std::list<T> *history_;

    inline size_t get_hash_pos_(const T &e) const;

    void hashset_resize_(size_t new_capacity); // with rehash
    void clear_();

    typename std::list<Entry<T>>::iterator list_find_(std::list<Entry<T>> &list, const T &e);

    typename std::list<Entry<T>>::iterator arr_find_(const T &e);

    void deep_delete_arr_();

    void deep_copy_arr_(const LinkedHashSet &other);
};


#endif //LINKEDHASHMAP_LINKEDHASHSET_H
