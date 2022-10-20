#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "LinkedHashSet.h"

template class LinkedHashSet<Path>;
template class LinkedHashSet<Student>;

template<class T>
LinkedHashSet<T>::LinkedHashSet() : elem_count_(0),
                                    arr_occupancy_(0),
                                    arr_capacity_(DEFAULT_CAPACITY_) {

    arr_ = new std::list<Entry<T>> *[arr_capacity_]();
    history_ = new std::list<T>();
}

template<class T>
LinkedHashSet<T>::LinkedHashSet(size_t capacity) : elem_count_(0),
                                                   arr_occupancy_(0),
                                                   arr_capacity_(capacity) {
    arr_ = new std::list<Entry<T>> *[arr_capacity_]();
    history_ = new std::list<T>();
}

template<class T>
LinkedHashSet<T>::~LinkedHashSet() {
    for (size_t i = 0; i < arr_capacity_ && arr_occupancy_ > 0; i++) {
        std::list<Entry<T>> *&cur_list = arr_[i];
        if (cur_list != nullptr) {
            arr_occupancy_ -= 1;
            delete cur_list;
        }
    }
    delete[] arr_;
    delete history_;
}

template<class T>
LinkedHashSet<T>::LinkedHashSet(const LinkedHashSet<T> &other) : elem_count_(other.elem_count_),
                                                                 arr_occupancy_(other.arr_occupancy_),
                                                                 arr_capacity_(other.arr_capacity_) {
    arr_ = nullptr;
    deep_copy_arr_(other);
    history_ = new std::list<T>(*other.history_);
}

template<class T>
LinkedHashSet<T> &LinkedHashSet<T>::operator=(const LinkedHashSet<T> &other) {
    if (this != &other) {
        deep_delete_arr_();
        deep_copy_arr_(other);
        delete history_;
        history_ = new std::list<T>(*other.history_);
    }
    return *this;
}

// 1. lhs1 insert insert (resize) -> capacity * 2
// 2. lhs1.remove()
// 3. lhs2 insert() -> capacity
template<class T>
bool LinkedHashSet<T>::operator==(const LinkedHashSet<T> &other) {
    if (this->elem_count_ != other.elem_count_)
        return false;

    auto it = this->begin();
    auto itEnd = this->end();
    while (it != itEnd) {
        if (!other.contains(*it)) {
            return false;
        }
        ++it;
    }

    return true;
}

template<class T>
bool LinkedHashSet<T>::operator!=(const LinkedHashSet<T> &other) {
    return !operator==(other);
}

// User-accessible interface.
template<class T>
bool LinkedHashSet<T>::insert(const T &e) {
    if (arr_occupancy_ > arr_capacity_ * OCCUPACITY_COEFFICIENT_) {
        if (arr_capacity_ * 2 <= UINT32_MAX) {
            hashset_resize_(arr_capacity_ * 2);
        }
    }

    size_t pos = get_hash_pos_(e);
    // Will cur_list evaluate arr_[pos] each time or will it store the result of this function?
    std::list<Entry<T>> *&cur_list = arr_[pos];
    if (cur_list == nullptr) {
        cur_list = new std::list<Entry<T>>();
        arr_occupancy_++;
    }

    // Existence check.
    if (list_find_(*cur_list, e) != cur_list->end()) {
        return false;
    } else {

        auto it = history_->insert(history_->end(), e);
        cur_list->emplace_back(Entry<T>(*it, it));

        elem_count_++;
        return true;
    }
}

template<class T>
bool LinkedHashSet<T>::remove(const T &e) {
    size_t pos = get_hash_pos_(e);
    std::list<Entry<T>> *&cur_list = arr_[pos];

    if (cur_list == nullptr)
        return false;

    auto curEntryIter = list_find_(*cur_list, e);
    if (curEntryIter == cur_list->end()) {
        return false;
    } else {
        history_->erase(curEntryIter->iterator_);
        cur_list->erase(curEntryIter);
        --elem_count_;
        return true;
    }
}

template<class T>
void LinkedHashSet<T>::deep_copy_arr_(const LinkedHashSet<T> &other) {
    arr_ = new std::list<Entry<T>> *[other.arr_capacity_]();

    arr_occupancy_ = other.arr_occupancy_;

    size_t i = 0;
    while (arr_occupancy_ > 0) {
        std::list<Entry<T>> *&list = other.arr_[i];
        if (list != nullptr) {
            arr_[i] = new std::list<Entry<T>>(*list);
            arr_occupancy_--;
        }
        i++;
    }

    arr_occupancy_ = other.arr_occupancy_;
    arr_capacity_ = other.arr_capacity_;
    elem_count_ = other.elem_count_;
}

template<class T>
void LinkedHashSet<T>::deep_delete_arr_() {
    size_t tmp_arr_occupancy_ = arr_occupancy_;
    size_t i = 0;
    while (tmp_arr_occupancy_ > 0) {
        std::list<Entry<T>> *&list = arr_[i++];
        if (list != nullptr) {
            tmp_arr_occupancy_--;
            delete list;
        }
    }
    delete[] arr_;
    arr_ = nullptr;
}

template<class T>
void LinkedHashSet<T>::clear_() {
    /* The Clear method calls the destructors for each element in the list,
     * but retains the current capacity.
     */
    for (int i = 0; i < arr_capacity_; ++i) {
        std::list<Entry<T>> *&list = arr_[i];
        if (list != nullptr) {
            delete list;
            --arr_occupancy_;
        }
        if (arr_occupancy_ == 0)
            break;
    }
    history_->clear();
    elem_count_ = 0;
}

template<class T>
inline size_t LinkedHashSet<T>::get_hash_pos_(const T &e) const {
    return e.hash() % arr_capacity_;
}

template<class T>
void LinkedHashSet<T>::hashset_resize_(size_t new_capacity) {
    arr_capacity_ = new_capacity;
    std::list<Entry<T>> **new_arr_ = new std::list<Entry<T>> *[new_capacity]();

    size_t tmp_arr_occupancy_ = 0;
    auto it = this->begin();
    auto itEnd = this->end();
    while (it != itEnd) {
        size_t pos = get_hash_pos_(*it);
        // Will new_list evaluate arr_[pos] each time or will it store the result of this function?
        std::list<Entry<T>> *&new_list = new_arr_[pos];
        if (new_list == nullptr) {
            new_list = new std::list<Entry<T>>();
            tmp_arr_occupancy_++;
        }

        new_list->emplace_back(Entry<T>(*it, it));
        ++it;
    }

    deep_delete_arr_();
    arr_ = new_arr_;
    arr_occupancy_ = tmp_arr_occupancy_;
}

template<class T>
void LinkedHashSet<T>::swap(LinkedHashSet<T> &other) {
    std::swap(arr_, other.arr_);
    std::swap(elem_count_, other.elem_count_);
    std::swap(arr_capacity_, other.arr_capacity_);
    std::swap(arr_occupancy_, other.arr_occupancy_);
    std::swap(history_, other.history_);
}

template<class T>
size_t LinkedHashSet<T>::size() const {
    return this->elem_count_;
}

template<class T>
bool LinkedHashSet<T>::empty() const {
    return this->elem_count_ == 0;
}

//
template<class T>
bool LinkedHashSet<T>::contains(const T &e) const {
    // CR: find
    std::list<Entry<T>> *&list = arr_[get_hash_pos_(e)];
    if (list != nullptr) {
        auto it = list->begin();
        auto itEnd = list->end();
        while (it != itEnd) {
            if ((*it).value_ == e) {
                return true;
            }
            ++it;
        }
    }

    return false;
}

template<class T>
inline typename std::list<T>::iterator LinkedHashSet<T>::begin() {
    return history_->begin();
}

template<class T>
inline typename std::list<T>::iterator LinkedHashSet<T>::end() {
    return history_->end();
}

template<class T>
LinkedHashSet<T> &LinkedHashSet<T>::clear() {
    clear_();
    return *this;
}

template<class T>
typename std::list<typename LinkedHashSet<T>::template Entry<T>>::iterator
LinkedHashSet<T>::list_find_(std::list<LinkedHashSet::Entry<T>> &list, const T &e) {
    auto it = list.begin();
    auto itEnd = list.end();
    while (it != itEnd) {
        if ((*it).value_ == e) {
            return it;
        }
        ++it;
    }
    return itEnd;
}

template<class T>
typename std::list<typename LinkedHashSet<T>::template Entry<T>>::iterator
LinkedHashSet<T>::arr_find_(const T &e) {
    size_t pos = get_hash_pos_(e);
    std::list<Entry<T>> *&list = arr_[pos];
    if (list == nullptr) {
        return list->end();
    }
    return list_find_(*list, e);
}

template<class T>
typename std::list<T>::iterator LinkedHashSet<T>::find(const T &e) {
    return std::find(this->begin(), this->end(), e);
}

template<class T>
template<class K>
LinkedHashSet<T>::Entry<K>::Entry(K &value, typename std::list<K>::iterator iterator): value_(value),
                                                                                       iterator_(iterator) {}

template<class T>
template<class K>
LinkedHashSet<T>::Entry<K>::Entry(const LinkedHashSet::Entry<K> &other): value_(other.value_),
                                                                         iterator_(other.iterator_) {}

template<class T>
template<class K>
bool LinkedHashSet<T>::Entry<K>::operator==(const LinkedHashSet::Entry<K> &other) const {
    return value_ == other.value_ && iterator_ == other.iterator_;
}

template<class T>
template<class K>
bool LinkedHashSet<T>::Entry<K>::operator!=(const LinkedHashSet::Entry<K> &other) const {
    return !operator==(other);
}
