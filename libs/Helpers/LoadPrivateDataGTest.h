#ifndef LINKEDHASHSET_LOADPRIVATEDATAGTEST_H
#define LINKEDHASHSET_LOADPRIVATEDATAGTEST_H

#pragma once

#include <cstdio>
#include "../Types/LinkedHashSet.h"

class LoadPrivateDataGTest {
public:
    static size_t get_arr_occupancy(LinkedHashSet<Student> &lhs);
    static size_t get_arr_capacity(LinkedHashSet<Student> &lhs);
};


#endif //LINKEDHASHSET_LOADPRIVATEDATAGTEST_H
