#ifndef LINKEDHASHMAP_LINKEDHASHSET_H
#define LINKEDHASHMAP_LINKEDHASHSET_H

#include <list>
#include <vector>
#include <algorithm>

template<typename T, typename Hasher=std::hash<T>>
class LinkedHashSet {
public:
    explicit LinkedHashSet(size_t capacity = DEFAULT_CAPACITY);

    ~LinkedHashSet();

    LinkedHashSet(const LinkedHashSet &other);

    /* Put element of T type in hashset. */
    bool insert(const T &e);

    /* Remove element from hashset. */
    bool remove(const T &e);

    void swap(LinkedHashSet &other);

    size_t size() const;

    bool empty() const;

    bool contains(const T &e);

    /* Restore hashset to original form (including size and capacity). */
    LinkedHashSet &clear();

    LinkedHashSet &operator=(LinkedHashSet other);

    bool operator==(LinkedHashSet &other);

    bool operator!=(LinkedHashSet &other);

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
        typename std::list<T>::iterator _iterator;
        T &_value;
    };

    /* Default (size of _arr/number of buckets) on initialization. */
    static const size_t DEFAULT_CAPACITY = 8;

    /* Coefficient which uses by public insert() and remove() for resizing _arr.
     * Describes ratio of elements to _arrCapacity.
     */
    constexpr static const double CAPACITY_COEFF = 0.85;

    /* Size of arr array, number of buckets. */
    size_t _arrCapacity;

    /* Container of buckets lists.
     * _arr buckets contain an Entry with an iterator to the object and object's reference for O(1) fast access.
     */
    std::list<Entry> **_arr;

    /* _history is a main storage of objects. */
    std::list<T> _history;

    Hasher _hasher;

    /* Returns an index of bucket in _arr, which contains Entry with e object.
     *
     * @param e         The object whose index is to be obtained. It will be hashed with std::hash().
     * @param capacity  Hash division module.
     */
    inline size_t _getHashPos(const T &e, size_t capacity) const;

    /* Deletes all buckets and _arr using delete and delete[] respectively.
     * Doesn't change _arr to nullptr.
     */
    inline void _deleteArr();

    /* Replaces _arr with a container of the same type with size newCapacity.
     * Uses delete[] for previous _arr.
     * Overwrites _arrCapacity with newCapacity.
     *
     * @param newCapacity   New _arrCapacity for _arr, which is needed after _resize().
     */
    void _resize(size_t newCapacity);

    /* Returns an iterator of the list pointing to the Entry containing e.
     *
     * @param list  Entry list where will be searched Entry with e.
     * @param e     Search object with type T.
     */
    inline typename std::list<Entry>::iterator _findInList(std::list<Entry> *list, const T &e) const;

    /* Returns an Entry list from arr with position pos.
     * If the list of Entry at index pos is nullptr, it will be initialized and placed in _arr at pos.
     *
     * @param arr   Bucket array. Must be initialized.
     * @param pos   Bucket index. Must be in [0, _arrCapacity].
     */
    inline std::list<Entry> &_getList(std::list<Entry> **arr, size_t pos);

    /* Puts T object from *it in the bucket from arr filling an Entry with a list<T> iterator it.
     * Allocates a bucket in the arr if needed.
     *
     * @param arr       Bucket array. Must be initialized.
     * @param it        An iterator pointing to the object being placed.
     * @param capacity  arr size.
     */
    void _insert(std::list<Entry> **arr, typename std::list<T>::iterator &it, size_t capacity);
};

#include "LinkedHashSet.hpp"


#endif //LINKEDHASHMAP_LINKEDHASHSET_H
