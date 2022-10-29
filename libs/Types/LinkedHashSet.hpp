#ifndef LINKEDHASHSET_LINKEDHASHSET_HPP
#define LINKEDHASHSET_LINKEDHASHSET_HPP

#include "LinkedHashSet.h"

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::LinkedHashSet(size_t capacity) :  arr_capacity_(capacity),
                                                            arr_(new std::list<Entry < T>> *[arr_capacity_]()) {
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::~LinkedHashSet() {
    delete_arr_();
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::LinkedHashSet(const LinkedHashSet &other) : arr_capacity_(other.arr_capacity_),
                                                                      history_(other.history_) {
    init_hashset(other.arr_capacity_);
}

/* Put element of T type in hashset. */
template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::insert(const T &e) {
    /* Rehash set if number of element it bigger than arr_capacity * CAPACITY_COEFF_. */
    if (this->size() >= arr_capacity_ * CAPACITY_COEFF_) {
        delete_arr_();
        init_hashset(arr_capacity_ * 2);
    }

    size_t pos = get_hash_pos_(e, arr_capacity_);
    /* Initialize bucket list in arr_[pos], if it's null. */
    std::list<Entry<T>> *cur_list = arr_[pos];
    if (cur_list == nullptr) {
        arr_[pos] = new std::list<Entry<T>>();
        cur_list = arr_[pos];
    }

    /* Check for element existence and insert, if hashset doesn't contain it. */
    if (std::find_if(cur_list->begin(), cur_list->end(),
                     [&e](const Entry<T> &x) { return x.value_ == e; })
        != cur_list->end()) {
        return false;
    } else {
        /* history_ is a storage of elements;
         * arr_ buckets contains an Entry with an iterator to the element and its pointer for O(1) fast access. */
        auto it = history_.insert(history_.end(), e);
        cur_list->emplace_back(Entry<T>(*it, it));
        return true;
    }
}

/* Remove element from hashset. */
template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::remove(const T &e) {
    size_t pos = get_hash_pos_(e, arr_capacity_);
    std::list<Entry<T>> *&cur_list = arr_[pos];

    if (cur_list == nullptr)
        return false;

    /* Get an iterator for the Entry with e, which helps to remove e in O(1) from history_. */
    auto curEntryIter = std::find_if(cur_list->begin(), cur_list->end(),
                                     [e](const Entry<T> &x) { return x.value_ == e; });

    if (curEntryIter == cur_list->end()) {
        return false;
    } else {
        history_.erase(curEntryIter->iterator_);
        cur_list->erase(curEntryIter);
        return true;
    }
}

template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::swap(LinkedHashSet &other) {
    std::swap(arr_, other.arr_);
    std::swap(arr_capacity_, other.arr_capacity_);
    std::swap(history_, other.history_);
}

template<typename T, typename Hasher>
size_t LinkedHashSet<T, Hasher>::size() const {
    return history_.size();
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::empty() const {
    return history_.empty();
}


template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::contains(const T &e) const {
    std::list<Entry<T>> *&list = arr_[get_hash_pos_(e, arr_capacity_)];
    /* Uses a lambda expression to search for an Entry by element. */
    if (list != nullptr)
        return list->end() !=
               std::find_if(list->begin(), list->end(), [&e](const Entry<T> &x) { return x.value_ == e; });
    return false;
}

/* Restore hashset to original form. */
template<typename T, typename Hasher>
LinkedHashSet<T, Hasher> &LinkedHashSet<T, Hasher>::clear() {
    delete_arr_();
    arr_capacity_ = DEFAULT_CAPACITY_;
    arr_ = new std::list<Entry<T>> *[DEFAULT_CAPACITY_]();
    history_.clear();
    return *this;
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher> &LinkedHashSet<T, Hasher>::operator=(const LinkedHashSet &other) {
    if (this != &other) {
        /* Rewrite history_ and restore hashset arr_ from new history_. */
        delete_arr_();
        history_ = other.history_;
        init_hashset(other.arr_capacity_);
    }
    return *this;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::operator==(const LinkedHashSet &other) {
    if (this->size() != other.size()) {
        return false;
    }

    for (T e: *this) {
        if (!other.contains(e)) {
            return false;
        }
    }
    return true;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::operator!=(const LinkedHashSet &other) {
    return !operator==(other);
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::find(const T &e) {
    return std::find(this->begin(), this->end(), e);
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::begin() {
    return history_.begin();
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::end() {
    return history_.end();
}

/* Returns index of bucket list in arr_;
 * Uses special Hasher class with () operator overridden to get hash and convert it to position in bucket array.
 * */
template<typename T, typename Hasher>
size_t LinkedHashSet<T, Hasher>::get_hash_pos_(const T &e, size_t capacity) const {
    return Hasher()(e) % capacity;
}

/* Restore arr_ from history_;
 * Overrides arr_ without delete[] for previous arr_;
 * Capacity must be higher than size of history.
 * */
template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::init_hashset(size_t capacity) {
    arr_ = new std::list<Entry<T>> *[capacity]();
    arr_capacity_ = capacity;

    std::list<T> list = history_;
    history_.clear();
    for (T e: list) {
        insert(e);
    }
}


template<typename T, typename Hasher>
inline void LinkedHashSet<T, Hasher>::delete_arr_() {
    deep_delete_sub_arr_();
    delete[] arr_;
}

/* Delete all lists from arr_ without nullptr replacement. */
template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::deep_delete_sub_arr_() {
    if (arr_ == nullptr)
        return;

    /* NOTICE: Optimization which uses number of elements is not able, cause unknown which lists wasn't be initialized. */
    for (size_t i = 0; i < arr_capacity_; i++) {
        std::list<Entry<T>> *&cur_list = arr_[i];
        delete cur_list;
    }
}

template<typename T, typename Hasher>
template<class K>
LinkedHashSet<T, Hasher>::Entry<K>::Entry(K &value, typename std::list<K>::iterator &iterator) : value_(value),
                                                                                                 iterator_(iterator) {
}

template<typename T, typename Hasher>
template<class K>
LinkedHashSet<T, Hasher>::Entry<K>::Entry(const LinkedHashSet::Entry<K> &other) : value_(other.value_),
                                                                                  iterator_(other.iterator_) {}

template<typename T, typename Hasher>
template<class K>
bool LinkedHashSet<T, Hasher>::Entry<K>::operator==(const LinkedHashSet::Entry<K> &other) const {
    return value_ == other.value_ && iterator_ == other.iterator_;
}

template<typename T, typename Hasher>
template<class K>
bool LinkedHashSet<T, Hasher>::Entry<K>::operator!=(const LinkedHashSet::Entry<K> &other) const {
    return !operator==(other);
}

#endif //LINKEDHASHSET_LINKEDHASHSET_HPP
