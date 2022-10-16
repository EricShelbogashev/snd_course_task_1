#include <iostream>
#include "../libs/Types/LinkedHashSet.h"

Student get_student_by_inx(size_t inx) {
    return Student(inx, "NAME " + std::to_string(inx));
}

int main() {
    LinkedHashSet hashSet1;
    LinkedHashSet hashSet2;
    Student studentM1(10000, "NAME -1");
    hashSet1.insert(studentM1);
    hashSet1.insert(studentM1);

    hashSet2.insert(studentM1);

    size_t i = 0;
    while (i < 10) {
        hashSet2.insert(get_student_by_inx(i));
        i++;
    }
    i = 0;
    while (i < 10) {
        hashSet2.remove(get_student_by_inx(i));
        i++;
    }
}
