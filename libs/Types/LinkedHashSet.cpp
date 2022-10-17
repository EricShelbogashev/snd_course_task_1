#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include "LinkedHashSet.h"
// Constructors and operators.

LinkedHashSet::LinkedHashSet() : elem_count_(0),
                                 arr_occupancy_(0),
                                 arr_capacity_(DEFAULT_CAPACITY_) {

    arr_ = new std::list<Entry<element>> *[arr_capacity_]();
    history_ = new std::list<element>();
}

LinkedHashSet::LinkedHashSet(size_t capacity) : elem_count_(0),
                                                arr_occupancy_(0),
                                                arr_capacity_(capacity) {
    arr_ = new std::list<Entry<element>> *[arr_capacity_]();
    history_ = new std::list<element>();
}

LinkedHashSet::~LinkedHashSet() {
    for (size_t i = 0; i < arr_capacity_ && arr_occupancy_ > 0; i++) {
        std::list<Entry<element>> *&cur_list = arr_[i];
        if (cur_list != nullptr) {
            arr_occupancy_ -= 1;
            delete cur_list;
        }
    }
    delete[] arr_;
}

LinkedHashSet::LinkedHashSet(const LinkedHashSet &other) : elem_count_(other.elem_count_),
                                                           arr_occupancy_(other.arr_occupancy_),
                                                           arr_capacity_(other.arr_capacity_),
                                                           history_(other.history_) {
    arr_ = nullptr;
    deep_copy_arr_(other);
}

LinkedHashSet &LinkedHashSet::operator=(const LinkedHashSet &other) {
    if (this != &other) {
        deep_delete_arr_();
        deep_copy_arr_(other);
        history_ = other.history_;
    }
    return *this;
}

// 1. lhs1 insert insert (resize) -> capacity * 2
// 2. lhs1.remove()
// 3. lhs2 insert() -> capacity
bool LinkedHashSet::operator==(const LinkedHashSet &other) {
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

bool LinkedHashSet::operator!=(const LinkedHashSet &other) {
    return !operator==(other);
}

// User-accessible interface.
bool LinkedHashSet::insert(const element &e) {
    if (arr_occupancy_ > arr_capacity_ * OCCUPACITY_COEFFICIENT_) {
        if (arr_capacity_ * 2 <= UINT32_MAX) {
            hashmap_resize_(arr_capacity_ * 2);
        }
    }

    size_t pos = get_hash_pos_(e);
    // Will cur_list evaluate arr_[pos] each time or will it store the result of this function?
    std::list<Entry<element>> *&cur_list = arr_[pos];
    if (cur_list == nullptr) {
        cur_list = new std::list<Entry<element>>();
        arr_occupancy_++;
    }

    // Existence check.
    if (list_find_(*cur_list, e) == cur_list->end()) {
        return false;
    } else {
        history_->emplace_back(e);

        auto it = history_->insert(history_->end(), e);
        cur_list->emplace_back(Entry<element>(*it, it));

        elem_count_++;
        return true;
/*

        std::cout << "Элемент: "<< e.age_ << " " << e.name_ << "| " << elem_count_ << std::endl;
        std::cout << "Адрес элемента: " << pointer1 << std::endl;
        std::cout << "Адрес элемента в истории: " << history_->back() << std::endl;
        std::cout << "Значение итератора: " << (**it).name_ << std::endl;
        std::cout << "Значение хеша: " << pos << std::endl;

        for (auto e: *history_) {
            std::cout << e->name_ << "[ " << e << " ] \n";
        }
        std::cout << std::endl << std::endl;
*/

    }
}

bool LinkedHashSet::remove(const element &e) {
    size_t pos = get_hash_pos_(e);
    std::list<Entry<element>> *&cur_list = arr_[pos];

    if (cur_list == nullptr)
        return false;

    auto curEntryIter = list_find_(*cur_list, e);
    if (curEntryIter == cur_list->end()) {
        return false;
    } else {
        history_->erase(curEntryIter->iterator_);
        cur_list->erase(curEntryIter);
        elem_count_--;
        return true;
    }
}

// Helpers.

void LinkedHashSet::deep_copy_arr_(const LinkedHashSet &other) {
    arr_ = new std::list<Entry<element>> *[other.arr_capacity_]();

    arr_occupancy_ = other.arr_occupancy_;

    size_t i = 0;
    while (arr_occupancy_ > 0) {
        std::list<Entry<element>> *&list = other.arr_[i];
        if (list != nullptr) {
            arr_[i] = new std::list<Entry<element>>(*list);
            arr_occupancy_--;
        }
        i++;
    }

    arr_occupancy_ = other.arr_occupancy_;
    arr_capacity_ = other.arr_capacity_;
    elem_count_ = other.elem_count_;
}

void LinkedHashSet::deep_delete_arr_() {
    size_t tmp_arr_occupancy_ = arr_occupancy_;
    size_t i = 0;
    while (tmp_arr_occupancy_ > 0) {
        std::list<Entry<element>> *&list = arr_[i++];
        if (list != nullptr) {
            tmp_arr_occupancy_--;
            delete list;
        }
    }
    delete[] arr_;
    arr_ = nullptr;
}

void LinkedHashSet::clear_() {
    /* The Clear method calls the destructors for each element in the list,
     * but retains the current capacity.
     */
    for (int i = 0; i < arr_capacity_; ++i) {
        std::list<Entry<element>> *&list = arr_[i];
        delete list;
    }
    history_->clear();
}


inline size_t LinkedHashSet::get_hash_pos_(const element &e) const {
    return e.hash() % arr_capacity_;
}

void LinkedHashSet::hashmap_resize_(size_t new_capacity) {
    arr_capacity_ = new_capacity;
    std::list<Entry<element>> **new_arr_ = new std::list<Entry<element>> *[new_capacity]();

    size_t tmp_arr_occupancy_ = 0;
    auto it = this->begin();
    auto itEnd = this->end();
    while (it != itEnd) {
        size_t pos = get_hash_pos_(*it);
        // Will new_list evaluate arr_[pos] each time or will it store the result of this function?
        std::list<Entry<element>> *&new_list = new_arr_[pos];
        if (new_list == nullptr) {
            new_list = new std::list<Entry<element>>();
            tmp_arr_occupancy_++;
        }

        new_list->emplace_back(Entry<element>(*it, it));
    }

    deep_delete_arr_();
    arr_ = new_arr_;
    arr_occupancy_ = tmp_arr_occupancy_;
}

void LinkedHashSet::swap(LinkedHashSet &other) {
    std::swap(arr_, other.arr_);
    std::swap(elem_count_, other.elem_count_);
    std::swap(arr_capacity_, other.arr_capacity_);
    std::swap(arr_occupancy_, other.arr_occupancy_);
    std::swap(history_, other.history_);
}

size_t LinkedHashSet::size() const {
    return this->elem_count_;
}

bool LinkedHashSet::empty() const {
    return this->elem_count_ == 0;
}

bool LinkedHashSet::contains(const element &e) const {
    // CR: find
    std::list<Entry<element>> *&list = arr_[get_hash_pos_(e)];
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

// ---------------------------------------------------------------------------------

inline std::list<element>::iterator LinkedHashSet::begin() {
    return history_->begin();
}


inline std::list<element>::iterator LinkedHashSet::end() {
    return history_->end();
}

LinkedHashSet &LinkedHashSet::clear() {
    clear_();
    return *this;
}

std::list<LinkedHashSet::Entry<element>>::iterator
LinkedHashSet::list_find_(std::list<LinkedHashSet::Entry<element>> &list, const element &e) {
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

std::list<LinkedHashSet::Entry<element>>::iterator
LinkedHashSet::arr_find_(const element &e) {
    size_t pos = get_hash_pos_(e);
    std::list<Entry<element>> *&list = arr_[pos];
    if (list == nullptr) {
        return list->end();
    }
    return list_find_(*list, e);
}

template<class T>
LinkedHashSet::Entry<T>::Entry(T &value, typename std::list<T>::iterator iterator): value_(value),
                                                                                    iterator_(iterator) {}

template<class T>
LinkedHashSet::Entry<T>::Entry(const Entry<T> &other): value_(other.value_),
                                                       iterator_(other.iterator_) {}

template<class T>
bool LinkedHashSet::Entry<T>::operator==(const LinkedHashSet::Entry<T> &other) const {
    return false;
}

template<class T>
bool LinkedHashSet::Entry<T>::operator!=(const LinkedHashSet::Entry<T> &other) const {
    return false;
}
