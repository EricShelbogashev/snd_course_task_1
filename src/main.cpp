#include <iostream>
#include "../libs/Types/LinkedHashSet.h"

Student get_student_by_inx(size_t inx) {
    return Student(inx, "NAME " + std::to_string(inx));
}

int main() {
    LinkedHashSet hashSet1;
    std::list<Student> tmp;
    for (size_t i = 6; i < 15; i++) {
        Student e = get_student_by_inx(i*i);
        hashSet1.insert(e);
        tmp.push_back(e);
    }

    Student e1 = get_student_by_inx(49);
    Student e2 = get_student_by_inx(64);
    hashSet1.remove(e1);
    hashSet1.remove(e2);

    tmp.remove(e1);
    tmp.remove(e2);

    auto listIt = tmp.begin();
    for (Student e: hashSet1) {
        std::cout << e.age_ << " " << e.name_ << "\t\t";
        std::cout << (*listIt).age_ << " " << (*listIt).name_ << std::endl;
        ++listIt;
        if (listIt == tmp.end())
            break;
    }

    auto mapIt = hashSet1.begin();
    for (Student e: tmp) {
        ++mapIt;
    }
}
