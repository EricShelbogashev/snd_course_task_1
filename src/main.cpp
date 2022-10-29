#include <iostream>
#include "../libs/Types/LinkedHashSet.h"
#include "../libs/Types/Hasher.h"

int main() {
    LinkedHashSet<Path, PathHasher> hashSet1;
    Path path1("/usr/home");
    Path path2("/etc/");

    hashSet1.insert(path1);
    hashSet1.insert(path2);
    for (Path e: hashSet1) {
        std::cout << e.get_path() << std::endl;
    }
}
