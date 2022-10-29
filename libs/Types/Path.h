#ifndef LINKEDHASHSET_PATH_H
#define LINKEDHASHSET_PATH_H

#include <string>

class Path {
public:
    explicit Path(std::string path);
    Path(const Path &other) = default;
    bool operator==(const Path & other) const;
    bool operator!=(const Path & other) const;
    std::string get_path() const;
private:
    std::string path_;
};

#endif //LINKEDHASHSET_PATH_H
