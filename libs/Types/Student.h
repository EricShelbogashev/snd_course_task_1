#ifndef LINKEDHASHMAP_STUDENT_H
#define LINKEDHASHMAP_STUDENT_H

#include <string>

struct Student {
    // New field
    Student() = default;
    Student(unsigned age, std::string name);
    // New field
    Student(const Student &other);
    bool operator==(const Student & other) const;
    bool operator!=(const Student & other) const;
    size_t hash() const;
    unsigned age_;
    std::string name_;
};

#endif
