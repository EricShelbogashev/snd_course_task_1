#ifndef LINKEDHASHSET_HASHER_HPP
#define LINKEDHASHSET_HASHER_HPP

size_t StudentHasher::operator()(const Student &e) const {
    return std::hash<std::string>{}(std::to_string(e.age_) + e.name_);
}

size_t PathHasher::operator()(const Path &e) const {
    return std::hash<std::string>{}(e.get_path());
}

#endif //LINKEDHASHSET_HASHER_HPP
