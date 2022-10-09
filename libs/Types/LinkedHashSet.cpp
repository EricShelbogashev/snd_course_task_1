#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "LinkedHashSet.h"
// Constructors and operators.

LinkedHashSet::LinkedHashSet() : elem_count_(0),
                                 arr_occupancy_(0),
                                 arr_capacity_(DEFAULT_CAPACITY_) {

    arr_ = new std::list<element> *[arr_capacity_];
}

LinkedHashSet::LinkedHashSet(size_t capacity) : elem_count_(0),
                                                arr_occupancy_(0),
                                                arr_capacity_(capacity) {

    arr_ = new std::list<element> *[arr_capacity_];
}

LinkedHashSet::~LinkedHashSet() {
    std::list<element> *cur_list;

    for (size_t i = 0; i < arr_capacity_ && elem_count_ > 0; i++) {
        cur_list = arr_[i];
        if (cur_list != nullptr) {
            elem_count_ -= cur_list->size();
            delete cur_list;
        }
    }
    delete[] arr_;
}

LinkedHashSet::LinkedHashSet(const LinkedHashSet &other) : elem_count_(other.elem_count_),
                                                           arr_occupancy_(other.arr_occupancy_),
                                                           arr_capacity_(other.arr_capacity_) {
    arr_ = nullptr;
    deep_copy_arr_(other);
}

LinkedHashSet &LinkedHashSet::operator=(const LinkedHashSet &other) {
    if (*this != other) {
        this->elem_count_ = other.elem_count_;
        this->arr_capacity_ = other.arr_capacity_;
        this->arr_occupancy_ = other.arr_occupancy_;
        deep_delete_arr_();
        deep_copy_arr_(other);
    }
    return *this;
}

// 1. lhs1 insert insert (resize) -> capacity * 2
// 2. lhs1.remove()
// 3. lhs2 insert() -> capacity
bool LinkedHashSet::operator==(const LinkedHashSet &other) const {
    if (this->elem_count_ != other.elem_count_)
        return false;

    LinkedHashSet::iterator it = this->begin();
    while (it != this->end()) {
        if (!other.contains(*it)) {
            return false;
        }
    }

    return true;
}

bool LinkedHashSet::operator!=(const LinkedHashSet &other) const {
    return !operator==(other);
}

// User-accessible interface.

/* [] check
    insert(Student("foo", 1)) -> true
    insert(Student("foo", 1)) -> false
    ASSERT_EQ(size(), 1)
*/
bool LinkedHashSet::insert(const element &e) {
    if (arr_occupancy_ > arr_capacity_ * OCCUPACITY_COEFFICIENT_) {
        hashmap_resize_(arr_capacity_ * 2);
    }

    std::list<element> *cur_list = arr_[get_hash_pos_(e)];
    if (cur_list == nullptr) {
        arr_[get_hash_pos_(e)] = new std::list<element>();
        cur_list = arr_[get_hash_pos_(e)];
        arr_occupancy_++;
    }

    // Existence check.
    if (std::find(cur_list->begin(), cur_list->end(), e) != cur_list->end()) {
        return false;
    } else {
        cur_list->push_back(e);
        elem_count_++;
        return true;
    }
}

bool LinkedHashSet::remove(const element &e) {
    std::list<element> *cur_list = arr_[get_hash_pos_(e)];

    // Existence check.
    if (std::find(cur_list->begin(), cur_list->end(), e) != cur_list->end()) {
        cur_list->remove(e);
        elem_count_--;
        return true;
    } else {
        return false;
    }
}

// Helpers.

void LinkedHashSet::deep_copy_arr_(const LinkedHashSet &other) {
    arr_ = new std::list<element> *[other.arr_capacity_];

    arr_occupancy_ = other.arr_occupancy_;

    size_t i = 0;
    while (arr_occupancy_ > 0) {
        std::list<element> *list = other.arr_[i];
        if (list != nullptr) {
            arr_[i] = new std::list<element>(*list);
            arr_occupancy_--;
        }
    }


    arr_occupancy_ = other.arr_occupancy_;
    arr_capacity_ = other.arr_capacity_;
    elem_count_ = other.elem_count_;
}

void LinkedHashSet::deep_delete_arr_() {
    size_t tmp_arr_occupancy_ = arr_occupancy_;
    size_t i = 0;
    while (tmp_arr_occupancy_ > 0) {
        std::list<element> *list = arr_[i];
        if (tmp_arr_occupancy_ <= 0) {
            break;
        }

        if (list != nullptr) {
            tmp_arr_occupancy_--;
            delete list;
        }
    }
    delete[] arr_;
}

void LinkedHashSet::clear_() {
    size_t i = 0;
    while (elem_count_ > 0) {
        std::list<element> *list = arr_[i];

        if (list != nullptr) {
            elem_count_ -= list->size();
            list->clear();
        }

        i++;
    }

    // Pair to '<=' condition.
    this->elem_count_ = 0;
}

/*
void LinkedHashSet::rehash_() {
    // std::list<element> * new_arr_ ...
    auto new_arr_ = new std::list<element> *[arr_capacity_];

    std::list<element> * cur_list;
    std::list<element> * new_list;

    size_t tmp_elem_count_ = elem_count_;
    for (int i = 0; i < arr_capacity_ && tmp_elem_count_ > 0; ++i) {
        cur_list = arr_[i];
        if (cur_list != nullptr) {
            while (!cur_list->empty()) {
                element & curElem = cur_list->front();
                cur_list->pop_front();
                new_list = new_arr_[get_hash_pos_(curElem)];
                if (new_list == nullptr) {
                    new_list = new std::list<element>();
                }
                new_list.push_back(curElem);
                tmp_elem_count_++;
            }
        }
    }
    deep_delete_arr_();
    arr_ = new_arr_;
}
*/

inline size_t LinkedHashSet::get_hash_pos_(const element &e) const {
    return e.hash() % arr_capacity_;
}

void LinkedHashSet::hashmap_resize_(size_t new_capacity) {
    // std::list<element> * new_arr_ ...
    arr_capacity_ = new_capacity;
    auto new_arr_ = new std::list<element> *[new_capacity];

    std::list<element> *cur_list;
    std::list<element> *new_list;

    size_t tmp_elem_count_ = elem_count_;
    size_t tmp_arr_occupancy_ = 0;
    size_t i = 0;
    while (tmp_elem_count_ > 0) {
        cur_list = arr_[i];
        if (cur_list != nullptr) {
            while (!cur_list->empty()) {
                element &curElem = cur_list->front();
                cur_list->pop_front();
                new_list = new_arr_[get_hash_pos_(curElem)];
                if (new_list == nullptr) {
                    new_list = new std::list<element>();
                    tmp_arr_occupancy_++;
                }
                new_list->push_back(curElem);
                tmp_elem_count_++;
            }
        }
        i++;
    }

    deep_delete_arr_();
    arr_occupancy_ = tmp_arr_occupancy_;
    arr_ = new_arr_;
}


/*
// old code; necessary to rewrite.
void LinkedHashSet::resize_(size_t new_capacity) {
    assert(new_capacity > capacity_);

    if (new_capacity > capacity_) {
        for (int i = this->capacity_; i < new_capacity; ++i) {
            this->arr_.emplace_back(list<element>());
        }
        this->capacity_ = new_capacity;
    }
}
*/

// Utils.

void LinkedHashSet::swap(LinkedHashSet &other) {
    // std::swap() wasn't used.
    std::list<element> **tmp_arr_ = arr_;
    size_t tmp_arr_capacity_ = arr_capacity_;
    size_t tmp_arr_occupancy_ = arr_occupancy_;
    size_t tmp_elem_count_ = elem_count_;

    arr_ = other.arr_;
    elem_count_ = other.elem_count_;
    arr_capacity_ = other.arr_capacity_;
    arr_occupancy_ = other.arr_occupancy_;

    other.arr_ = tmp_arr_;
    other.elem_count_ = tmp_elem_count_;
    other.arr_capacity_ = tmp_arr_capacity_;
    other.arr_occupancy_ = tmp_arr_occupancy_;
}

size_t LinkedHashSet::size() const {
    return this->elem_count_;
}

bool LinkedHashSet::empty() const {
    return this->elem_count_ == 0;
}

bool LinkedHashSet::contains(const element &e) const {
    // CR: find
    std::list<element> *list = arr_[get_hash_pos_(e)];
    if (list != nullptr)
        return std::find(list->begin(), list->end(), e) != list->end();

    return false;
}

// ---------------------------------------------------------------------------------
LinkedHashSet::iterator LinkedHashSet::find(const element &e) const {
    return {};
}

LinkedHashSet::iterator LinkedHashSet::begin() const {
    return {};
}

LinkedHashSet::iterator LinkedHashSet::end() const {
    return {};
}


element LinkedHashSet::iterator::operator*() {
    return {};
}

LinkedHashSet::iterator LinkedHashSet::iterator::operator++(int) {
    return LinkedHashSet::iterator();
}

LinkedHashSet::iterator LinkedHashSet::iterator::operator--() {
    return {};
}

bool LinkedHashSet::iterator::operator==(const LinkedHashSet::iterator &other) const {
    return false;
}

bool LinkedHashSet::iterator::operator!=(const LinkedHashSet::iterator &other) const {
    return false;
}


// ---------------------------------------------------------------------------------

