#ifndef LINKEDHASHSET_LINKEDHASHSET_HPP
#define LINKEDHASHSET_LINKEDHASHSET_HPP

#include "LinkedHashSet.h"
#include <cassert>

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::LinkedHashSet(size_t capacity) :  arr_capacity_(capacity),
                                                            arr_(new std::list<Entry> *[arr_capacity_]()) {
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::~LinkedHashSet() {
    delete_arr_();
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::LinkedHashSet(const LinkedHashSet &other) : arr_capacity_(other.arr_capacity_),
                                                                      history_(other.history_),
                                                                      hasher_(other.hasher_),
                                                                      arr_(nullptr) {
    resize_(other.arr_capacity_);
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher> &LinkedHashSet<T, Hasher>::operator=(LinkedHashSet other) {
    if (*this == other)
        return *this;
    swap(other);
    return *this;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::insert(const T &e) {
    if (this->size() >= arr_capacity_) {
        resize_(arr_capacity_ * 2);
    }

    if (contains(e)) {
        return false;
    }
    auto it = history_.insert(history_.end(), e);
    insert_(arr_, e, it, arr_capacity_);
    return true;
}


template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::remove(const T &e) {
    if (arr_capacity_ * (1 - CAPACITY_COEFF_) > history_.size() && arr_capacity_ > DEFAULT_CAPACITY_) {
        resize_(arr_capacity_ / 2);
    }

    size_t pos = get_hash_pos_(e, arr_capacity_);
    std::list<Entry> *cur_list = arr_[pos];

    if (cur_list == nullptr)
        return false;

    auto curEntryIter = is_in_list_(cur_list, e);

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
    std::swap(hasher_, other.hasher_);
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
    size_t pos = get_hash_pos_(e, arr_capacity_);
    std::list<Entry> *list = arr_[pos];
    return list != nullptr && is_in_list_(list, e) != list->end();
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher> &LinkedHashSet<T, Hasher>::clear() {
    delete_arr_();
    arr_capacity_ = DEFAULT_CAPACITY_;
    arr_ = new std::list<Entry> *[DEFAULT_CAPACITY_]();
    history_.clear();
    return *this;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::operator==(const LinkedHashSet &other) {
    if (this->size() != other.size()) {
        return false;
    }

    auto predicate = [&other](T &e) { return !other.contains(e); };
    return std::none_of(this->begin(), this->end(), predicate);
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::operator!=(const LinkedHashSet &other) {
    return !operator==(other);
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::find(const T &e) {
    size_t pos = get_hash_pos_(e);
    std::list<LinkedHashSet<T, Hasher>::Entry> list = arr_[pos];
    return is_in_list_(list, e);
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::begin() {
    return history_.begin();
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::end() {
    return history_.end();
}

template<typename T, typename Hasher>
size_t LinkedHashSet<T, Hasher>::get_hash_pos_(const T &e, size_t capacity) const {
    return hasher_(e) % capacity;
}

template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::resize_(size_t new_capacity) {
    delete_arr_();
    auto new_arr = new std::list<Entry> *[new_capacity]();
    for (auto it = history_.begin(); it != history_.end(); ++it) {
        insert_(new_arr, *it, it, new_capacity);
    }
    arr_ = new_arr;
    arr_capacity_ = new_capacity;
}

template<typename T, typename Hasher>
inline void LinkedHashSet<T, Hasher>::delete_arr_() {
    if (this->arr_ == nullptr)
        return;

    for (size_t i = 0; i < arr_capacity_; i++) {
        delete arr_[i];
    }
    delete[] this->arr_;
}

template<typename T, typename Hasher>
inline typename std::list<typename LinkedHashSet<T, Hasher>::Entry>::iterator
LinkedHashSet<T, Hasher>::is_in_list_(std::list<Entry> *list, const T &e) const {
    assert(list != nullptr);
    return std::find_if(list->begin(), list->end(), [e](const Entry &x) { return x.value_ == e; });
}

template<typename T, typename Hasher>
inline std::list<typename LinkedHashSet<T, Hasher>::Entry> &
LinkedHashSet<T, Hasher>::get_list(std::list<Entry> **arr, size_t pos) {
    std::list<typename LinkedHashSet<T, Hasher>::Entry> *cur_list = arr[pos];
    if (cur_list == nullptr) {
        arr[pos] = new std::list<typename LinkedHashSet<T, Hasher>::Entry>();
        cur_list = arr[pos];
    }
    return *cur_list;
}

template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::insert_(std::list<Entry> **arr, const T &e, typename std::list<T>::iterator &it,
                                       size_t capacity) {
    size_t pos = get_hash_pos_(e, capacity);
    std::list<Entry> &cur_list = get_list(arr, pos);
    cur_list.emplace_back(Entry(*it, it));
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::Entry::Entry(T &value, typename std::list<T>::iterator &iterator) : value_(value),
                                                                                              iterator_(iterator) {
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::Entry::Entry(const LinkedHashSet::Entry &other) : value_(other.value_),
                                                                            iterator_(other.iterator_) {}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::Entry::operator==(const LinkedHashSet::Entry &other) const {
    return value_ == other.value_ && iterator_ == other.iterator_;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::Entry::operator!=(const LinkedHashSet::Entry &other) const {
    return !operator==(other);
}

#endif //LINKEDHASHSET_LINKEDHASHSET_HPP
