#ifndef LINKEDHASHMAP_HASHUTILS_H
#define LINKEDHASHMAP_HASHUTILS_H


#include <string>

class HashUtils {
public:
    static long long int stringHash(std::string const &string);
};

// CR: namespace
int stringHash(std::string const &string);

#endif