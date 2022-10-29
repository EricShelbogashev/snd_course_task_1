#include "Student.h"

#include <utility>

Student::Student(unsigned age, std::string name) : age_(age), name_(std::move(name)) {}

Student::Student(const Student &other) = default;

bool Student::operator==(const Student &other) const {
    return this->name_ == other.name_ && this->age_ == other.age_;
}

bool Student::operator!=(const Student &other) const {
    return !operator==(other);
}