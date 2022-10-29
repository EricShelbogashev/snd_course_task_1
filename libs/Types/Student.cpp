#include "Student.h"

Student::Student(unsigned age, std::string name) : age_(age), name_(name) {}

Student::Student(const Student &other) : age_(other.age_), name_(other.name_) {}

bool Student::operator==(const Student &other) const {
    return this->name_ == other.name_ && this->age_ == other.age_;
}

bool Student::operator!=(const Student &other) const {
    return !operator==(other);
}