#ifndef LINKEDHASHSET_HASHER_H
#define LINKEDHASHSET_HASHER_H

#include <iostream>
#include <string>
#include "Student.h"
#include "Path.h"

/* Hasher interface */
template<class T>
class Hasher {
    virtual size_t operator()(const T &e) const = 0;
};

class StudentHasher : public Hasher<Student> {
public:
    size_t operator()(const Student &e) const override;
};

class PathHasher : public Hasher<Path> {
public:
    size_t operator()(const Path &e) const override;
};

#include "Hasher.hpp"

#endif //LINKEDHASHSET_HASHER_H
