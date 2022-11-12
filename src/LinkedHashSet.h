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

    // CR: is new copy independent from the old one?
    LinkedHashSet(const LinkedHashSet &other);

    /* Put element of T type in hashset. */
    // CR: when this method returns true? when it returns false? mention in doc
    bool insert(const T &e);

    /* Remove element from hashset. */
    // CR: same as insert
    bool remove(const T &e);

    void swap(LinkedHashSet &other);

    size_t size() const;

    bool empty() const;

    bool contains(const T &e);

    /* Restore hashset to original form (including size and capacity). */
    LinkedHashSet &clear();

    LinkedHashSet &operator=(LinkedHashSet other);

    // CR: comment? also mention that order does not matter
    bool operator==(LinkedHashSet &other);

    bool operator!=(LinkedHashSet &other);

    typename std::list<T>::iterator find(const T &e);

    typename std::list<T>::iterator begin();

    typename std::list<T>::iterator end();

private:
    /* Default (size of _arr/number of buckets) on initialization. */
    static const size_t DEFAULT_CAPACITY = 8;

    /* Coefficient which uses by public insert() and remove() for resizing _arr.
     * Describes ratio of elements to _arrCapacity.
     */
    constexpr static const double CAPACITY_COEFF = 0.85;

    /* Size of arr array, number of buckets. */
    size_t _arrCapacity;

    /* Container of buckets lists.
     * _arr buckets contain an _history iterator to the object for O(1) fast access.
     * bucket := _history iterator list
     */
    // CR: maybe rename to _buckets? not insisting though
    std::list<typename std::list<T>::iterator> **_arr;

    /* _history is a main storage of objects. */
    std::list<T> _history;

    Hasher _hasher;

    /* Returns an index of bucket in _arr, which contains _history iterator to e object.
     *
     * @param e         The object whose index is to be obtained. It will be hashed with std::hash().
     * @param capacity  Hash division module.
     */
    // CR: I usually prefer a clear naming instead of lots of comments
    // CR: e.g. this method can be renamed to _getBucketIdx.
    // CR: Also comments are not verified, so if you described method logic in comment
    // CR: and then changed this logic, comment is not verified automatically, so it can became stale pretty fast
    // CR: This is only a recommendation, it's up to you if you want to follow it :)
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

    /* Returns an iterator of the bucket pointing to the _history iterator to e.
     *
     * @param list  list where will be searched _history iterator to e.
     * @param e     Search object with type T.
     */
    inline typename std::list<typename std::list<T>::iterator>::iterator
    _findInList(std::list<typename std::list<T>::iterator> *list, const T &e) const;

    /* Returns an bucket from arr with position pos.
     * If the bucket at index pos is nullptr, it will be initialized and placed in _arr at pos.
     *
     * @param arr   Bucket array. Must be initialized.
     * @param pos   Bucket index. Must be in [0, _arrCapacity].
     */
    inline std::list<typename std::list<T>::iterator> &
    _getList(std::list<typename std::list<T>::iterator> **arr, size_t pos);

    /* Puts T object from *it in the bucket from arr filling an iterator it.
     * Allocates a bucket in the arr if needed.
     *
     * @param arr       Bucket array. Must be initialized.
     * @param it        An iterator pointing to the object being placed.
     * @param capacity  arr size.
     */
    void
    _insert(std::list<typename std::list<T>::iterator> **arr, typename std::list<T>::iterator &it, size_t capacity);
};

#include "LinkedHashSet.hpp"


#endif //LINKEDHASHMAP_LINKEDHASHSET_H
