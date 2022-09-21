#include "Student.h"
#include "../Utils/HashUtils.h"

#include <utility>

Student::Student(unsigned int age, std::string name) {
    this->age_ = age;
    this->name_ = std::move(name);
}

bool Student::operator==(const Student &other) const {
    // In the model, Student objects are equal if they have the same name and the same age.
    return (this->name_ == other.name_) && (this->age_ == other.age_);
}

long long Student::hash() const {
    return HashUtils::stringHash(std::to_string(this->age_) + this->name_);
}