#include <gtest/gtest.h>
#include "../libs/Types/LinkedHashSet.h"
#include "../libs/Helpers/LoadPrivateDataGTest.h"

namespace testingHelpers {
    Student get_student_by_inx(size_t inx) {
        return Student(inx, "NAME " + std::to_string(inx));
    }
}

TEST(InsertMethodTest, InsertSameElements) {
    LinkedHashSet<Student> hashSet;

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

TEST(InsertMethodTest, RemoveSameElements) {
    LinkedHashSet<Student> hashSet;
    Student student1(300, "NAME 1");
    hashSet.insert(student1);
    hashSet.remove(student1);
    ASSERT_EQ(hashSet.remove(student1), false) << "insert(s1); remove(s1); remove(s1) -> false";
}

TEST(InsertMethodTest, InsertLargeTest) {
    LinkedHashSet<Student> hashSet2;
    for (size_t i = 0; i < 100000; i++) {
        hashSet2.insert(testingHelpers::get_student_by_inx(i));
        ASSERT_EQ(hashSet2.size(), i + 1);
    }

    ASSERT_EQ(hashSet2.size(), 100000);
}

TEST(EqualityTest, InsertSameElements) {
    LinkedHashSet<Student> hashSet1;
    LinkedHashSet<Student> hashSet2;
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
    ASSERT_EQ(hashSet1 != hashSet2, false) << "hashSet1 and hashSet2 must be equal";
}

TEST(EqualityTest, DifferentCapacities) {
    LinkedHashSet<Student> hashSet1;
    LinkedHashSet<Student> hashSet2(100);
    ASSERT_EQ(hashSet1 == hashSet2, true);
    ASSERT_EQ(hashSet1 != hashSet2, false);
}

TEST(EqualityTest, CopyCtor) {
    LinkedHashSet<Student>hashSet1;
    for (int i = 5; i < 15; i += 2) {
        hashSet1.insert(testingHelpers::get_student_by_inx(i));
    }
    LinkedHashSet<Student>hashSet2(hashSet1);
    ASSERT_EQ(hashSet1 == hashSet2, true);
    ASSERT_EQ(hashSet1 != hashSet2, false);
}

TEST(CopyConstructorTest, LargeCopy) {
    LinkedHashSet<Student>hashSet;
    for (size_t i = 0; i < 100000; i++) {
        hashSet.insert(testingHelpers::get_student_by_inx(i));
    }
    LinkedHashSet<Student>hashSetCopy(hashSet);
    ASSERT_EQ(hashSet == hashSetCopy, true);
}

TEST(ClearMethodTest, ClearedAndEmptyEquality) {
    LinkedHashSet<Student>hashSet;
    for (size_t i = 0; i < 10; i++) {
        hashSet.insert(testingHelpers::get_student_by_inx(i));
    }

    for (size_t i = 5; i < 10; i++) {
        hashSet.remove(testingHelpers::get_student_by_inx(i));
    }
    LinkedHashSet<Student>hashSetEmpty;
    hashSet.clear();
    ASSERT_EQ(hashSet == hashSetEmpty, true);
}

TEST(RemoveMethodTest, RemoveNonExistent) {
    LinkedHashSet<Student>hashSet;
    for (size_t i = 0; i < 10; i++) {
        hashSet.insert(testingHelpers::get_student_by_inx(i));
    }
    hashSet.remove(Student(0, "NAME"));
    ASSERT_EQ(hashSet.size(), 10);
}

TEST(RemoveMethodTest, RemoveTwice) {
    LinkedHashSet<Student>hashSet;
    for (size_t i = 0; i < 9; i++) {
        hashSet.insert(testingHelpers::get_student_by_inx(i));
    }
    hashSet.remove(testingHelpers::get_student_by_inx(2));
    hashSet.remove(testingHelpers::get_student_by_inx(1));
    ASSERT_EQ(hashSet.size(), 7);
}

TEST(SwapMethodTest, PairEquality) {
    LinkedHashSet<Student>lhs1;
    for (size_t i = 0; i < 10; i++) {
        lhs1.insert(testingHelpers::get_student_by_inx(i));
    }
    LinkedHashSet<Student>lhs1Pair(lhs1);

    LinkedHashSet<Student>lhs2;
    for (size_t i = 20; i < 50; i += 2) {
        lhs2.insert(testingHelpers::get_student_by_inx(i));
    }

    LinkedHashSet<Student>lhs2Pair(lhs2);
    ASSERT_EQ(
            (lhs1 == lhs1Pair) && (lhs2 == lhs2Pair)
            && (lhs1 != lhs2Pair) && (lhs2 != lhs1Pair)
            && (lhs1Pair != lhs2Pair) && (lhs1 != lhs2),
            true
    ) << "Bad input condition";

    lhs1.swap(lhs2);

    ASSERT_EQ(lhs1 == lhs2Pair, true);
    ASSERT_EQ(lhs2 == lhs1Pair, true);
    ASSERT_EQ(lhs1 != lhs2, true);
}

TEST(SwapMethodTest, SwapTwiceLarge) {
    LinkedHashSet<Student>lhs1;
    for (size_t i = 0; i < 10000; i++) {
        lhs1.insert(testingHelpers::get_student_by_inx(i));
    }
    LinkedHashSet<Student>lhs1Pair(lhs1);

    LinkedHashSet<Student>lhs2;
    for (size_t i = 10000; i < 14000; i++) {
        lhs2.insert(testingHelpers::get_student_by_inx(i));
    }

    lhs2.swap(lhs1);
    lhs1.swap(lhs2);
    ASSERT_EQ(lhs1 == lhs1Pair, true);
    ASSERT_EQ(lhs1 != lhs2, true);
}

TEST(SizeMethodTest, SimpleSizeCheck) {
    LinkedHashSet<Student>lhs1;
    size_t counter = 0;
    for (size_t i = 1; i < 100000; i++) {
        counter += 1;
        lhs1.insert(testingHelpers::get_student_by_inx(i * i));
        ASSERT_EQ(lhs1.size(), counter);
    }
}

TEST(ContainsMethodTest, SimpleContainsCheck) {
    LinkedHashSet<Student>hashSet;
    Student student1(300, "NAME 1");
    hashSet.insert(student1);
    ASSERT_EQ(hashSet.contains(student1), true);
}

TEST(ContainsMethodTest, MultiplyInsert) {
    LinkedHashSet<Student>hashSet;
    Student student1(300, "NAME 1");
    hashSet.insert(student1);
    Student student2(4300, "Bar 1");
    hashSet.insert(student1);
    hashSet.insert(student2);
    ASSERT_EQ(hashSet.contains(student1), true);
    ASSERT_EQ(hashSet.contains(student2), true);
}

TEST(ContainsMethodTest, MultiplyInsertAndMultiplyRemove) {
    LinkedHashSet<Student>hashSet;
    Student student1(300, "NAME 1");
    Student student2(4300, "Bar 1");
    hashSet.insert(student1);
    hashSet.insert(student2);
    hashSet.insert(student2);
    hashSet.insert(student1);
    for (size_t i = 40; i < 56; i++) {
        hashSet.insert(testingHelpers::get_student_by_inx(i * i));
    }
    hashSet.remove(student2);

    ASSERT_EQ(hashSet.contains(student1), true);
    ASSERT_EQ(hashSet.contains(student2), false);
}

TEST(AssignmentOperatorTest, LargeAssignment) {
    LinkedHashSet<Student>hashSet1;
    for (size_t i = 0; i < 100000; i++) {
        hashSet1.insert(testingHelpers::get_student_by_inx(i));
    }
    LinkedHashSet<Student>hashSet1Pair(hashSet1);
    LinkedHashSet<Student>hashSet2;
    for (size_t i = 6; i < 15; i++) {
        hashSet2.insert(testingHelpers::get_student_by_inx(i));
    }

    hashSet2 = hashSet1;
    ASSERT_EQ(hashSet2 == hashSet1Pair, true);
}

TEST(AssignmentOperatorTest, SelfAssignment) {
    LinkedHashSet<Student>hashSet1;
    for (size_t i = 6; i < 15; i++) {
        hashSet1.insert(testingHelpers::get_student_by_inx(i*i));
    }
    LinkedHashSet<Student>hashSet1Pair(hashSet1);
    hashSet1 = hashSet1;
    ASSERT_EQ(hashSet1 == hashSet1Pair, true);
}

TEST(IteratorTest, KeepingOrderCheck) {
    LinkedHashSet<Student>hashSet1;
    std::list<Student> tmp;
    for (size_t i = 6; i < 15; i++) {
        Student e = testingHelpers::get_student_by_inx(i*i);
        hashSet1.insert(e);
        tmp.push_back(e);
    }

    auto listIt = tmp.begin();
    for (Student e: hashSet1) {
        ASSERT_EQ(e, *listIt);
        ++listIt;
    }

    auto mapIt = hashSet1.begin();
    for (Student e: tmp) {
        ASSERT_EQ(e, *mapIt);
        ++mapIt;
    }
}

TEST(IteratorTest, KeepingOrderCheckWithRemove) {
    LinkedHashSet<Student>hashSet1;
    std::list<Student> tmp;
    for (size_t i = 6; i < 15; i++) {
        Student e = testingHelpers::get_student_by_inx(i*i);
        hashSet1.insert(e);
        tmp.push_back(e);
    }

    Student e1 = testingHelpers::get_student_by_inx(49);
    Student e2 = testingHelpers::get_student_by_inx(64);
    hashSet1.remove(e1);
    hashSet1.remove(e2);

    tmp.remove(e1);
    tmp.remove(e2);

    auto listIt = tmp.begin();
    for (Student e: hashSet1) {
        ASSERT_EQ(e == *listIt, true);
        ++listIt;
    }

    auto mapIt = hashSet1.begin();
    for (Student e: tmp) {
        ASSERT_EQ(e, *mapIt);
        ++mapIt;
    }
}