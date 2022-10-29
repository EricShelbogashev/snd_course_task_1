#include "Path.h"

#include <utility>

Path::Path(std::string path): path_(std::move(path)) {}

bool Path::operator==(const Path &other) const {
    return path_ == other.path_;
}

bool Path::operator!=(const Path &other) const {
    return !operator==(other);
}

std::string Path::get_path() const {
    return path_;
}
