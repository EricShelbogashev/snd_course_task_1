#include "Path.h"
#include "../Helpers/HashUtils.h"

Path::Path(std::string path): path_(path) {}

Path::Path(const Path &other): path_(other.path_) {}

bool Path::operator==(const Path &other) const {
    return path_ == other.path_;
}

bool Path::operator!=(const Path &other) const {
    return !operator==(other);
}

size_t Path::hash() const {
    return hash::stringHash(path_);
}

std::string Path::get_path() {
    return path_;
}
