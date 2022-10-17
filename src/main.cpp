#include <iostream>
#include "../libs/Types/Path.h"
#include "../libs/Types/Student.h"
#include "../libs/Types/LinkedHashSet.h"

int main() {
    LinkedHashSet<Path> hashSet1;
    Path path1("/usr/home");
    Path path2("/etc/");

    hashSet1.insert(path1);
    hashSet1.insert(path2);
    for (Path e: hashSet1) {
        std::cout << e.get_path() << std::endl;
    }
}
