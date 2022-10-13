#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include "LinkedHashSet.h"
#include "Pair.h"
// Constructors and operators.

LinkedHashSet::LinkedHashSet() : elem_count_(0),
                                 arr_occupancy_(0),
                                 arr_capacity_(DEFAULT_CAPACITY_) {

    arr_ = new std::list<element> *[arr_capacity_]();
}

LinkedHashSet::LinkedHashSet(size_t capacity) : elem_count_(0),
                                                arr_occupancy_(0),
                                                arr_capacity_(capacity) {
    arr_ = new std::list<element> *[arr_capacity_]();
}

LinkedHashSet::~LinkedHashSet() {
    for (size_t i = 0; i < arr_capacity_ && arr_occupancy_ > 0; i++) {
        std::list<element> *&cur_list = arr_[i];
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
    // Будет ли cur_list каждый раз выполнять arr_[pos] или он сохранит результат этой функции?
    std::list<element> *&cur_list = arr_[pos];
    if (cur_list == nullptr) {
        cur_list = new std::list<element>();
        arr_occupancy_++;
    }

    // Existence check.
    if (std::find(cur_list->begin(), cur_list->end(), e) != cur_list->end()) {
        return false;
    } else {
        history_.push_back(Pair<size_t>(pos, cur_list->size()));
        cur_list->push_back(e);
        elem_count_++;
        return true;
    }
}

bool LinkedHashSet::remove(const element &e) {
    size_t pos = get_hash_pos_(e);
    std::list<element> *&cur_list = arr_[pos];

    if (cur_list == nullptr)
        return false;

    auto itList = cur_list->begin();
    auto itListEnd = cur_list->end();
    size_t listPos = 0;
    while (itList != itListEnd) {
        if (*itList == e) {
            break;
        }
        listPos++;
        ++itList;
    }

    if (itList != itListEnd) {
        history_.remove(Pair<size_t>(pos, listPos));
        cur_list->remove(e);
        elem_count_--;
        return true;
    } else {
        return false;
    }
}

// Helpers.

void LinkedHashSet::deep_copy_arr_(const LinkedHashSet &other) {
    arr_ = new std::list<element> *[other.arr_capacity_]();

    arr_occupancy_ = other.arr_occupancy_;

    size_t i = 0;
    while (arr_occupancy_ > 0) {
        std::list<element> *&list = other.arr_[i];
        if (list != nullptr) {
            arr_[i] = new std::list<element>(*list);
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
        std::list<element> *&list = arr_[i++];
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
        std::list<element> *&list = arr_[i];

        if (list != nullptr) {
            elem_count_ -= list->size();
            list->clear();
        }

        i++;
    }
    history_.clear();
}


inline size_t LinkedHashSet::get_hash_pos_(const element &e) const {
    return e.hash() % arr_capacity_;
}

void LinkedHashSet::hashmap_resize_(size_t new_capacity) {
    arr_capacity_ = new_capacity;
    auto new_arr_ = new std::list<element> *[new_capacity]();

    size_t tmp_arr_occupancy_ = 0;
    std::list<element> *new_list;
    std::list<Pair<size_t>> history_tmp_;
    auto it = this->begin();
    size_t pos;
    auto itEnd = this->end();
    while (it != itEnd) {
        pos = get_hash_pos_(*it);
        new_list = new_arr_[pos];
        if (new_list == nullptr) {
            new_arr_[pos] = new std::list<element>();
            new_list = new_arr_[pos];
            tmp_arr_occupancy_++;
        }
        history_tmp_.push_back(Pair<size_t>(pos, new_arr_[pos]->size()));
        new_list->push_back(*it);
        ++it;
    }

    deep_delete_arr_();
    history_ = history_tmp_;
    arr_occupancy_ = tmp_arr_occupancy_;
    arr_ = new_arr_;
}

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
LinkedHashSet::iterator::iterator(std::list<Pair<size_t>>::iterator it, LinkedHashSet *p) : hist_iter_(it), lhs(p) {}

LinkedHashSet::iterator LinkedHashSet::find(const element &e) {
    size_t pos_a = this->get_hash_pos_(e);
    std::list<element> *cur_list = arr_[pos_a];
    size_t pos_b = 0;
    auto it = cur_list->begin();
    // Will be create copy of cur_list->end() for each of iterations?
    while (*it != e && it != cur_list->end()) {
        it++;
        pos_b++;
    }
    if (it == cur_list->end()) {
        return LinkedHashSet::iterator(history_.end(), this);
    }
    Pair<size_t> cur_pair(pos_a, pos_b);
    auto it2 = history_.begin();
    while (
            (*it2).get_first() != cur_pair.get_first()
            && (*it2).get_second() != cur_pair.get_second()
            ) {
        it2++;
    }
    return LinkedHashSet::iterator(it2, this);
}


LinkedHashSet::iterator LinkedHashSet::begin() {
    return LinkedHashSet::iterator(history_.begin(), this);
}


LinkedHashSet::iterator LinkedHashSet::end() {
    return LinkedHashSet::iterator(history_.end(), this);
}

LinkedHashSet &LinkedHashSet::clear() {
    clear_();
    return *this;
}

element LinkedHashSet::iterator::operator*() {
    Pair<size_t> &cur_pos = *hist_iter_;
    std::list<element> *cur_list = lhs->arr_[cur_pos.get_first()];
    auto lit = cur_list->begin();
    std::advance(lit, cur_pos.get_second());
    return *lit;
}

LinkedHashSet::iterator &LinkedHashSet::iterator::operator++() {
    ++hist_iter_;
    return *this;
}

LinkedHashSet::iterator LinkedHashSet::iterator::operator++(int) {
    LinkedHashSet::iterator tmp(*this);
    ++hist_iter_;
    return tmp;
}

LinkedHashSet::iterator &LinkedHashSet::iterator::operator--() {
    hist_iter_--;
    return *this;
}

bool LinkedHashSet::iterator::operator==(const LinkedHashSet::iterator &other) const {
    return hist_iter_ == other.hist_iter_ && lhs == other.lhs;
}

bool LinkedHashSet::iterator::operator!=(const LinkedHashSet::iterator &other) const {
    return !operator==(other);
}
