#include <cassert>
#include "LinkedHashSet.h"

using namespace std;

// Constructors.

LinkedHashSet::LinkedHashSet() {
    this->size_ = 0;
    this->capacity_ = 64;
    for (int i = 0; i < capacity_; ++i) {
        this->arr_.push_back(list<element>());
    }
}

LinkedHashSet::~LinkedHashSet() = default;

LinkedHashSet::LinkedHashSet(const LinkedHashSet &other) {
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;

    // Will there be a memory leak issue here?
    this->arr_ = std::vector<std::list<element>>(other.arr_);
}

// User-accessible interface.

bool LinkedHashSet::insert(const element &e) {
    long long pos = getHashPos_(&e);

    if ((float) size_ > (float) capacity_ * occupancy_) {
        int tmp = this->capacity_;
        this->arr_.resize(capacity_ * 2);
        this->rehash_(tmp);
    }

    this->arr_.at(pos).push_back(e);
    this->size_++;

    return true;
}

bool LinkedHashSet::remove(const element &e) {
    long long pos = getHashPos_(&e);
    this->arr_.at(pos).remove(e);
    this->size_--;
    return true;
}

// Core utils.

void LinkedHashSet::clear_() {
    arr_.clear();
    for (int i = 0; i < capacity_; ++i) {
        arr_[i].clear();
    }
    this->size_ = 0;
}

void LinkedHashSet::rehash_(int old_capacity) {
    auto tmp = new vector<list<element>>();
    tmp->reserve(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        this->arr_.push_back(list<element>());
    }

    list<element> *curList;
    element *curElem;
    for (int i = 0; i < old_capacity; ++i) {
        curList = &arr_.at(i);
        while (!curList->empty()) {
            curElem = &curList->front();
            tmp->at(getHashPos_(curElem)).push_back(*curElem);
        }
    }

    // Will there be a memory leak issue here?
    // Should we do ```delete[] this->arr_``` ?
    // Does a copy of the vector appear here by pointer?
    this->arr_ = *tmp;
}

long long LinkedHashSet::getHashPos_(const element *e) const {
    return e->hash() % capacity_;
}

void LinkedHashSet::resize_(int new_capacity) {
    assert(new_capacity > capacity_);

    if (new_capacity > capacity_) {
        for (int i = this->capacity_; i < new_capacity; ++i) {
            this->arr_.push_back(list<element>());
        }
        this->capacity_ = new_capacity;
    }
}

// Utils.

void LinkedHashSet::swap(LinkedHashSet &other) {
    LinkedHashSet &tmp = *this;
    *this = other;
    other = tmp;
}

size_t LinkedHashSet::size() const {
    return this->size_;
}

bool LinkedHashSet::empty() const {
    return this->size_ == 0;
}

bool LinkedHashSet::contains(const element &e) const {
    long long pos = getHashPos_(&e);

    for (element elem: arr_.at(pos)) {
        if (elem == e) return true;
    }
    return false;
}

// Operators.

LinkedHashSet &LinkedHashSet::operator=(const LinkedHashSet &other) {
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;

    // Will there be a memory leak issue here?
    this->arr_ = std::vector<std::list<element>>(other.arr_);
    return *this;
}

bool LinkedHashSet::operator==(const LinkedHashSet &other) const {
    /* Define equality as a match of elements.
     * */
    if (this->size_ != other.size_)
        return false;

    for (int i = 0; i < capacity_; ++i) {
        if (this->arr_.at(i).size() == other.arr_.at(i).size()) {
            auto curListIterThis = this->arr_.at(i).begin();
            auto curListIterOther = this->arr_.at(i).begin();
            while (curListIterThis != this->arr_.at(i).end()) {
                if (curListIterThis != curListIterOther) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }

    return true;
}

bool LinkedHashSet::operator!=(const LinkedHashSet &other) const {
    return !operator==(other);
}