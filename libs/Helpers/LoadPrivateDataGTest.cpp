#include "LoadPrivateDataGTest.h"

size_t LoadPrivateDataGTest::get_arr_occupancy(LinkedHashSet<Student> &lhs) {
    return lhs.arr_occupancy_;
}

size_t LoadPrivateDataGTest::get_arr_capacity(LinkedHashSet<Student> &lhs) {
    return lhs.arr_capacity_;
}
