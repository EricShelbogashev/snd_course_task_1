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

    /*! @brief   Initializes the hashset with same elements and same inserted order with \p other.
     *  @details Doesn't depend on \p other by everything
     *
     *  @param   other   hashset to be copied to independent object.
     */
    LinkedHashSet(const LinkedHashSet &other);

    /*! @brief   Inserts an element into hashset.
     *
     *  @return  `true`    if hashset doesn't contain the element.
     *  @return  `false`   otherwise.
     *
     *  @param   e       element to be inserted to hashset.
     */
    bool insert(const T &e);

    /*! @brief   Remove element from hashset.
     *
     *  @return  `true`    if hashset contains the element.
     *  @return  `false`   otherwise.
     *
     *  @param  e        element to be removed from hashset.
     */
    bool remove(const T &e);

    void swap(LinkedHashSet &other);

    size_t size() const;

    bool empty() const;

    bool contains(const T &e);

    /*! @brief   Restore hashset to original form (including size and capacity). */
    LinkedHashSet &clear();

    LinkedHashSet &operator=(const LinkedHashSet &other);

    /*! @brief   Classical in understanding the equivalence relation between hashsets.
     *  @details Doesn't depend on the order of elements.
     *
     *  @return  `true`    if hashsets contain same elements.
     *  @return  `false`   otherwise.
     *
     *  @param   other   hashset to be compared to *this.
     */
    bool operator==(LinkedHashSet &other);

    bool operator!=(LinkedHashSet &other);

    typename std::list<T>::iterator find(const T &e);

    typename std::list<T>::iterator begin();

    typename std::list<T>::iterator end();

private:
    /*! @brief   Default (size of #_buckets/number of buckets) on initialization. */
    static const size_t DEFAULT_CAPACITY = 8;

    /*! @brief   Coefficient which uses by public #insert() and #remove() for resizing #_buckets.
     *  @details Describes ratio of elements to #_arrCapacity.
     */
    constexpr static const double CAPACITY_COEFF = 0.85;

    /*! @brief   Size of arr array, number of buckets. */
    size_t _arrCapacity;

    /*! @brief   Container of buckets lists.
     *  @details #_buckets buckets contain an #_history iterator to the object for O(1) fast access.
     *           bucket := #_history iterator list
     */
    std::list<typename std::list<T>::iterator> **_buckets;

    /*! @brief   #_history is a main storage of objects. */
    std::list<T> _history;

    Hasher _hasher;

    /*! @return          an index of bucket in #_buckets, which contains #_history iterator to \p e object.
     *
     *  @param e         The object whose index is to be obtained. It will be hashed with std::hash().
     *  @param capacity  Hash division module.
     */
    inline size_t _getBucketIdx(const T &e, size_t capacity) const;

    /*! @brief   Deletes all buckets and #_buckets using **delete** and **delete[]** respectively.
     *  @details Doesn't change #_buckets to nullptr.
     */
    inline void _deleteArr();

    /*! @brief   Replaces #_buckets with a container of the same type with size \p newCapacity.
     *  @details Uses **delete[]** for previous #_buckets.
     *           Overwrites #_arrCapacity with \p newCapacity.
     *
     *  @param newCapacity   New #_arrCapacity for #_buckets, which is needed after #_resize().
     */
    void _resize(size_t newCapacity);

    /*! @return      an iterator of the bucket pointing to the #_history iterator to \p e.
     *
     *  @param list  \p list where will be searched #_history iterator to \p e.
     *  @param e     Search object with type T.
     */
    inline typename std::list<typename std::list<T>::iterator>::iterator
    _findInList(std::list<typename std::list<T>::iterator> *list, const T &e) const;

    /*! @return      a bucket from \p bucketsArr with position \p pos.
     *  @details     If the bucket at index \p pos is nullptr, it will be initialized and
     *               placed in \p bucketsArr at \p pos.
     *
     *  @param bucketsArr    Bucket array. Must be initialized.
     *  @param pos           Bucket index. Must be in [0, #_arrCapacity].
     */
    inline std::list<typename std::list<T>::iterator> &
    _getList(std::list<typename std::list<T>::iterator> **bucketsArr, size_t pos);

    /*! @brief   Puts T object from *(it) in the bucket from \p bucketsArr filling an iterator \p it.
     *  @details Allocates a bucket in the \p bucketsArr if needed.
     *
     *  @param bucketsArr    Bucket array. Must be initialized.
     *  @param it            An iterator pointing to the object being placed.
     *  @param capacity      \p bucketsArr size.
     */
    void
    _insert(std::list<typename std::list<T>::iterator> **bucketsArr, typename std::list<T>::iterator &it,
            size_t capacity);
};

#include "LinkedHashSet.hpp"


#endif //LINKEDHASHMAP_LINKEDHASHSET_H
