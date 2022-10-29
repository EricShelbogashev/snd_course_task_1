#ifndef LINKEDHASHMAP_STUDENT_H
#define LINKEDHASHMAP_STUDENT_H

#include <string>

struct Student {
    Student() = default;
    Student(unsigned age, std::string name);
    Student(const Student &other);
    bool operator==(const Student & other) const;
    bool operator!=(const Student & other) const;
    unsigned int age_ = 0;
    std::string name_;
};

#endif
