#include "../LinkedHashSet.h"

// Constructors

/*Iterator::Iterator(LinkedHashSet &lHS) {
    this->size_position_ = 0;
    this->hash_position_ = 0;
    this->curr_iter_ = lHS.arr_.at(0).begin();
}*/

/*Iterator Iterator::operator++(LinkedHashSet &lHS, int) {
    curr_iter_++;
    size_position_++;
    if (curr_iter_ == lHS.arr_.at(hash_position_).end()) {
        if (size_position_ == lHS.size_) {
            return *this;
        }

        while (lHS.arr_.at(hash_position_).empty() && (hash_position_ < lHS.capacity_)) {
            hash_position_++;
        }

        // If all works correctly, this situation is impossible.
        *//*if (hash_position_ == lHS.capacity_) {
            return *this;
        }*//*

        curr_iter_ = lHS.arr_.at(hash_position_).begin();
        return *this;
    } else {
        curr_iter_++;
        return *this;
    }
}*/

