#include <cassert>
#include "LinkedHashSet.h"

using namespace std;

// Constructors.

LinkedHashSet::LinkedHashSet() : size_(0), capacity_(16) {
    for (int i = 0; i < capacity_; ++i) {
        this->arr_.emplace_back(list<element>());
    }
}

LinkedHashSet::~LinkedHashSet() = default;

// CR: init list
LinkedHashSet::LinkedHashSet(const LinkedHashSet &other) {
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;
    this->arr_ = other.arr_;
}

LinkedHashSet &LinkedHashSet::operator=(const LinkedHashSet &other) {
    // CR: *this != other
    this->size_ = other.size_;
    this->capacity_ = other.capacity_;
    this->arr_ = other.arr_;
    return *this;
}

// User-accessible interface.

bool LinkedHashSet::insert(const element &e) {
    size_t pos = getHashPos_(&e);

    if (size_ > capacity_ * OCCUPANCY_) {
        int tmp = this->capacity_;
        this->arr_.resize(capacity_ * 2);
        this->rehash_(tmp);
    }

    // CR:
    // insert(Student("foo", 1)) -> true
    // insert(Student("foo", 1)) -> false
    // ASSERT_EQ(size(), 1)
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
    for (int i = 0; i < capacity_; ++i) {
        // CR:
        // if size_ == 0 break;
        // size_ -= arr[i].size()
        arr_[i].clear();
    }
    this->size_ = 0;
}

void LinkedHashSet::rehash_(int old_capacity) {
    auto tmp = new vector<list<element>>();
    tmp->reserve(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        this->arr_.emplace_back(list<element>());
    }

    // CR:
    // for (auto & list : arr_) {
    //   for (auto & elem: list) {
    //     tmp[getHashPos_(&elem)].push_back(elem);
    //   }
    // }

    list<element> * curList;
    element *curElem;
    for (int i = 0; i < old_capacity; ++i) {
        curList = &arr_.at(i);
        while (!curList->empty()) {
            curElem = &curList->front();
            tmp->at(getHashPos_(curElem)).push_back(*curElem);
        }
    }

    this->arr_ = *tmp;
}

// CR: size_t
// CR: better to pass reference
inline long long LinkedHashSet::getHashPos_(const element *e) const {
    return e->hash() % capacity_;
}

void LinkedHashSet::resize_(int new_capacity) {
    assert(new_capacity > capacity_);

    if (new_capacity > capacity_) {
        for (int i = this->capacity_; i < new_capacity; ++i) {
            this->arr_.emplace_back(list<element>());
        }
        this->capacity_ = new_capacity;
    }
}

// Utils.

// hs1: linkedhs (s1)
// hs2: linkedhs (s2)
// hs1.swap(hs2) 
// hs1: linkedhs (s2)
// hs2: linkedhs (s1)
void LinkedHashSet::swap(LinkedHashSet &other) {
    // CR: std::swap()
    LinkedHashSet tmp = *this;
    // hs2: linkedhs (s2), hs1: linkedhs (s2)
    *this = other;
    // hs2: linkedhs (s2), hs1: linkedhs (s2)
    other = tmp;
}

size_t LinkedHashSet::size() const {
    return this->size_;
}

bool LinkedHashSet::empty() const {
    return this->size_ == 0;
}

bool LinkedHashSet::contains(const element &e) const {
  // CR: find
    long long pos = getHashPos_(&e);

    for (element elem: arr_.at(pos)) {
        if (elem == e) return true;
    }
    return false;
}

// Operators.


bool LinkedHashSet::operator==(const LinkedHashSet &other) const {
    /* Define equality as a match of elements.
     * */
    if (this->size_ != other.size_)
        return false;

    // 1. lhs1 insert insert (resize) -> capacity * 2
    // 2. lhs1.remove()
    // 3. lhs2 insert() -> capacity
    // CR: iterate e = this->arr, other.contains(e)
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

int main() {}