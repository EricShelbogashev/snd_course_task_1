#ifndef LINKEDHASHSET_HASHER_H
#define LINKEDHASHSET_HASHER_H

#include <iostream>
#include <string>
#include "Student.h"
#include "Path.h"

class StringHasher {
public:
    size_t operator()(const Student &e) const {
        return std::hash<std::string>{}(std::to_string(e.age_) + e.name_);
    }
};

class PathHasher {
public:
    size_t operator()(const Path &e) const {
        return std::hash<std::string>{}(e.get_path());
    }
};

#endif //LINKEDHASHSET_HASHER_H
