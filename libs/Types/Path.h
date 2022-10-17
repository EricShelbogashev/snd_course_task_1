#ifndef LINKEDHASHSET_PATH_H
#define LINKEDHASHSET_PATH_H

#include <string>

struct Path {
public:
    Path(std::string path);
    Path(const Path &other);
    bool operator==(const Path & other) const;
    bool operator!=(const Path & other) const;
    size_t hash() const;
    std::string get_path();
private:
    std::string path_;
};

#endif //LINKEDHASHSET_PATH_H
