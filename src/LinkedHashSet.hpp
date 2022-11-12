#ifndef LINKEDHASHSET_LINKEDHASHSET_HPP
#define LINKEDHASHSET_LINKEDHASHSET_HPP

#include "LinkedHashSet.h"
#include <cassert>

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::LinkedHashSet(size_t capacity) :  _arrCapacity(capacity),
                                                            _arr(new std::list<typename std::list<T>::iterator> *[_arrCapacity]()) {
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::~LinkedHashSet() {
    _deleteArr();
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher>::LinkedHashSet(const LinkedHashSet &other) : _arrCapacity(other._arrCapacity),
                                                                      _history(other._history),
                                                                      _hasher(other._hasher),
                                                                      _arr(nullptr) {
    _resize(other._arrCapacity);
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher> &LinkedHashSet<T, Hasher>::operator=(LinkedHashSet other) {
    if (*this == other) {
        return *this;
    }
    swap(other);
    return *this;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::insert(const T &e) {
    if (contains(e)) {
        return false;
    }
    if (this->size() >= _arrCapacity) {
        _resize(_arrCapacity * 2);
    }

    auto it = _history.insert(_history.end(), e);
    _insert(_arr, it, _arrCapacity);
    return true;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::remove(const T &e) {
    size_t pos = _getHashPos(e, _arrCapacity);
    std::list<typename std::list<T>::iterator> *cur_list = _arr[pos];

    if (cur_list == nullptr) {
        return false;
    }

    auto curEntryIter = _findInList(cur_list, e);

    if (curEntryIter == cur_list->end()) {
        return false;
    }
    _history.erase(*curEntryIter);
    if (_arrCapacity * (1 - CAPACITY_COEFF) > _history.size() && _arrCapacity > DEFAULT_CAPACITY) {
        _resize(_arrCapacity / 2);
    } else {
        // CR: better to erase in both branches, because _resize may change and will forget to update callers
        cur_list->erase(curEntryIter);
    }
    return true;
}

template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::swap(LinkedHashSet &other) {
    std::swap(_arr, other._arr);
    std::swap(_arrCapacity, other._arrCapacity);
    std::swap(_history, other._history);
    std::swap(_hasher, other._hasher);
}

template<typename T, typename Hasher>
size_t LinkedHashSet<T, Hasher>::size() const {
    return _history.size();
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::empty() const {
    return _history.empty();
}


template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::contains(const T &e) {
    return find(e) != this->end();
}

template<typename T, typename Hasher>
LinkedHashSet<T, Hasher> &LinkedHashSet<T, Hasher>::clear() {
    _deleteArr();
    _arrCapacity = DEFAULT_CAPACITY;
    _arr = new std::list<typename std::list<T>::iterator> *[DEFAULT_CAPACITY]();
    _history.clear();
    return *this;
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::operator==(LinkedHashSet &other) {
    if (this->size() != other.size()) {
        return false;
    }

    auto predicate = [&other](T &e) { return !other.contains(e); };
    return std::none_of(this->begin(), this->end(), predicate);
}

template<typename T, typename Hasher>
bool LinkedHashSet<T, Hasher>::operator!=(LinkedHashSet &other) {
    return !operator==(other);
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::find(const T &e) {
    size_t pos = _getHashPos(e, _arrCapacity);
    std::list<typename std::list<T>::iterator> *list = _arr[pos];
    if (list == nullptr) {
        return this->end();
    }
    auto entryIter = _findInList(list, e);
    return entryIter == list->end() ? this->end() : *entryIter;
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::begin() {
    return _history.begin();
}

template<typename T, typename Hasher>
typename std::list<T>::iterator LinkedHashSet<T, Hasher>::end() {
    return _history.end();
}

template<typename T, typename Hasher>
size_t LinkedHashSet<T, Hasher>::_getHashPos(const T &e, size_t capacity) const {
    return _hasher(e) % capacity;
}

template<typename T, typename Hasher>
void LinkedHashSet<T, Hasher>::_resize(size_t newCapacity) {
    _deleteArr();
    auto new_arr = new std::list<typename std::list<T>::iterator> *[newCapacity]();
    for (auto it = _history.begin(); it != _history.end(); ++it) {
        _insert(new_arr, it, newCapacity);
    }
    _arr = new_arr;
    _arrCapacity = newCapacity;
}

template<typename T, typename Hasher>
inline void LinkedHashSet<T, Hasher>::_deleteArr() {
    if (this->_arr == nullptr) {
        return;
    }
    for (size_t i = 0; i < _arrCapacity; i++) {
        delete _arr[i];
    }
    delete[] this->_arr;
}

template<typename T, typename Hasher>
inline typename std::list<typename std::list<T>::iterator>::iterator
      LinkedHashSet<T, Hasher>::_findInList(std::list<typename std::list<T>::iterator> *list, const T &e) const {
    assert(list != nullptr);
    return std::find_if(list->begin(), list->end(), [e](const typename std::list<T>::iterator &x) { return *x == e; });
}

template<typename T, typename Hasher>
inline std::list<typename std::list<T>::iterator> &
LinkedHashSet<T, Hasher>::_getList(std::list<typename std::list<T>::iterator> **arr, size_t pos) {
    assert(arr != nullptr);
    std::list<typename std::list<T>::iterator> *curList = arr[pos];
    if (curList == nullptr) {
        arr[pos] = new std::list<typename std::list<T>::iterator>();
        curList = arr[pos];
    }
    return *curList;
}

template<typename T, typename Hasher>
void
LinkedHashSet<T, Hasher>::_insert(std::list<typename std::list<T>::iterator> **arr, typename std::list<T>::iterator &it, size_t capacity) {
    size_t pos = _getHashPos(*it, capacity);
    std::list<typename std::list<T>::iterator> &curList = _getList(arr, pos);
    curList.emplace_back(it);
}

#endif //LINKEDHASHSET_LINKEDHASHSET_HPP
