#include "Path.h"

Path::Path(std::string path): path_(path) {}

bool Path::operator==(const Path &other) const {
    return path_ == other.path_;
}

bool Path::operator!=(const Path &other) const {
    return !operator==(other);
}

std::string Path::get_path() const {
    return path_;
}
