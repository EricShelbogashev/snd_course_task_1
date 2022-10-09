#include <iostream>
#include "../libs/Types/LinkedHashSet.h"

int main() {
    LinkedHashSet hashSet;
    Student student1 = {19, "Eric"};
    Student student2 = {19, "Lida"};
    Student student3 = {18, "Lekyshkaa"};
    hashSet.insert(student1);
    hashSet.insert(student1);
    hashSet.insert(student2);
    hashSet.insert(student2);
    hashSet.insert(student3);
    hashSet.remove(student1);
    hashSet.remove(student1);
    hashSet.remove(student2);
    std::cout << "hashSet.contains(student1): " << hashSet.contains(student1) << std::endl;
    std::cout << "hashSet.contains(student2): " << hashSet.contains(student2) << std::endl;
    std::cout << "hashSet.contains(student3): " << hashSet.contains(student3) << std::endl;
    std::cout << "hashSet.size(): " << hashSet.size() << std::endl;
}
