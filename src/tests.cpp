#include <gtest/gtest.h>
#include <iostream>
#include "../libs/Types/LinkedHashSet.h"
#include "../libs/Types/Student.h"

TEST(InsertTest, InsertSameElements) {
    LinkedHashSet hashSet;
    Student student1(300, "NAME 1");
    Student student2(0, "NAME 2");
    hashSet.insert(student1);
    ASSERT_EQ(hashSet.remove(student1), true) << "hashSet.insert(student1); hashSet.remove(student1) must returned true";
    hashSet.remove(student1);
    ASSERT_EQ(hashSet.remove(student1), false) << "hashSet.insert(student1); hashSet.remove(student1); hashSet.remove(student1) must returned false";
    hashSet.remove(student2);
}