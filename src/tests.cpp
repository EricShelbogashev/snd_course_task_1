#include <gtest/gtest.h>
#include <iostream>
#include "../libs/Types/LinkedHashSet.h"
#include "../libs/Types/Student.h"
#include "../libs/Helpers/LoadPrivateDataGTest.h"

namespace testingHelpers {
    Student get_student_by_inx(int inx) {
        return Student(inx, "NAME " + std::to_string(inx));
    }
}

TEST(InsertTest, InsertSameElements) {
    LinkedHashSet hashSet;

    Student student2(0, "NAME 2");
    hashSet.insert(student2);
    hashSet.insert(student2);
    ASSERT_EQ(hashSet.remove(student2), true) << "insert(s1); insert(s1); remove(s1) -> true";

    Student student3(60, "NAME 3");
    size_t tmp = hashSet.size();
    hashSet.insert(student3);
    hashSet.insert(student3);
    hashSet.remove(student3);
    ASSERT_EQ(hashSet.size(), tmp) << "size_t tmp = size(); \ninsert(s1); \ninsert(s1); \nremove(s1); \nsize() -> tmp";

    Student student4(9999, "NAME 4");
    hashSet.insert(student4);
    tmp = hashSet.size();
    hashSet.insert(student4);
    ASSERT_EQ(hashSet.size(), tmp) << "insert(s1); \nsize_t tmp = size(); \ninsert(s1); \nsize() -> tmp";
}

TEST(InsertTest, RemoveSameElements) {
    LinkedHashSet hashSet;
    Student student1(300, "NAME 1");
    hashSet.insert(student1);
    hashSet.remove(student1);
    ASSERT_EQ(hashSet.remove(student1), false) << "insert(s1); remove(s1); remove(s1) -> false";
}

TEST(EqualityTest, InsertSameElements) {
    LinkedHashSet hashSet1;
    LinkedHashSet hashSet2;
    Student studentM1(10000, "NAME -1");
    hashSet1.insert(studentM1);
    hashSet1.insert(studentM1);

    hashSet2.insert(studentM1);

    size_t i = 0;
    size_t cap_diff = LoadPrivateDataGTest::get_arr_capacity(hashSet1)
            - LoadPrivateDataGTest::get_arr_occupancy(hashSet1) + 2;
    while (i < cap_diff * 2) {
        hashSet2.insert(testingHelpers::get_student_by_inx(i));
        i++;
    }
    i = 0;
    while (i < cap_diff * 2) {
        hashSet2.remove(testingHelpers::get_student_by_inx(i));
        i++;
    }

    ASSERT_EQ(hashSet1 == hashSet2, true) << "hashSet1 and hashSet2 must be equal";
}