#ifndef TASK1_STUDENT_H
#define TASK1_STUDENT_H

#include <string>

struct Student {
    // New field
    Student() = default;
    Student(unsigned age, std::string name);
    // New field
    Student(const Student &other);
    bool operator==(const Student & other) const;
    long long hash() const;
    unsigned age_;
    std::string name_;
};

#endif
