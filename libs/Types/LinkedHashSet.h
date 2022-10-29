#ifndef LINKEDHASHMAP_LINKEDHASHSET_H
#define LINKEDHASHMAP_LINKEDHASHSET_H

#include <list>
#include <vector>
#include <algorithm>
#include "Student.h"
#include "Path.h"

template<typename T, typename Hasher=std::hash<T>>
class LinkedHashSet {
public:

    explicit LinkedHashSet(size_t capacity = DEFAULT_CAPACITY_) : arr_capacity_(capacity) {
        arr_ = new std::list<Entry<T>> *[arr_capacity_]();
    }

    ~LinkedHashSet() {
        delete_arr_();
    }

    LinkedHashSet(const LinkedHashSet &other) : arr_capacity_(other.arr_capacity_),
                                                history_(other.history_),
                                                arr_(nullptr) {
        init_hashset(other.arr_capacity_);
    }

    bool insert(const T &e) {
        if (this->size() >= arr_capacity_ * CAPACITY_COEFF_) {
            delete_arr_();
            init_hashset(arr_capacity_ * 2);
        }

        size_t pos = get_hash_pos_(e, arr_capacity_);
        std::list<Entry<T>> *cur_list = arr_[pos];
        if (cur_list == nullptr) {
            arr_[pos] = new std::list<Entry<T>>();
            cur_list = arr_[pos];
        }

        if (std::find_if(cur_list->begin(), cur_list->end(),
                         [&e](const Entry<T> &x) { return x.value_ == e; })
            != cur_list->end()) {
            return false;
        } else {
            auto it = history_.insert(history_.end(), e);
            cur_list->emplace_back(Entry<T>(*it, it));
            return true;
        }
    }

    bool remove(const T &e) {
        size_t pos = get_hash_pos_(e, arr_capacity_);
        std::list<Entry<T>> *&cur_list = arr_[pos];

        if (cur_list == nullptr)
            return false;

        auto curEntryIter = std::find_if(cur_list->begin(), cur_list->end(),
                                         [e](const Entry<T> &x) { return x.value_ == e; });

        if (curEntryIter == cur_list->end()) {
            return false;
        } else {
            history_.erase(curEntryIter->iterator_);
            cur_list->erase(curEntryIter);
            return true;
        }
    }

    void swap(LinkedHashSet &other) {
        std::swap(arr_, other.arr_);
        std::swap(arr_capacity_, other.arr_capacity_);
        std::swap(history_, other.history_);
    }

    size_t size() const {
        return history_.size();
    }

    bool empty() const {
        return history_.empty();
    }

    bool contains(const T &e) const {
        std::list<Entry<T>> *&list = arr_[get_hash_pos_(e, arr_capacity_)];
        if (list != nullptr)
            return list->end() != std::find_if(list->begin(), list->end(), [&e](const Entry<T> &x) { return x.value_ == e; });
        return false;
    }

    LinkedHashSet &clear() {
        delete_arr_();
        arr_capacity_ = DEFAULT_CAPACITY_;
        arr_ = new std::list<Entry<T>> *[DEFAULT_CAPACITY_]();
        history_.clear();
    }

    LinkedHashSet &operator=(const LinkedHashSet &other) {
        if (this != &other) {
            delete_arr_();
            history_ = other.history_;
            init_hashset(other.arr_capacity_);
        }
        return *this;
    }

    bool operator==(const LinkedHashSet &other) {
        if (this->size() != other.size()) {
            return false;
        }

        for (T e: *this) {
            if (!other.contains(e)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const LinkedHashSet &other) {
        return !operator==(other);
    }

    typename std::list<T>::iterator find(const T &e) {
        return std::find(this->begin(), this->end(), e);
    }

    typename std::list<T>::iterator begin() {
        return history_.begin();
    }

    typename std::list<T>::iterator end() {
        return history_.end();
    }

private:
    template<class K>
    class Entry {
    public:
        explicit Entry(K &value, typename std::list<K>::iterator iterator) : value_(value),
                                                                             iterator_(iterator) {}

        Entry(const Entry<K> &other) : value_(other.value_),
                                       iterator_(other.iterator_) {}

        bool operator==(const Entry<K> &other) const {
            return value_ == other.value_ && iterator_ == other.iterator_;
        }

        bool operator!=(const Entry<K> &other) const {
            return !operator==(other);
        }

    private:
        friend LinkedHashSet;
        typename std::list<K>::iterator iterator_;
        K &value_;
    };

    static const size_t DEFAULT_CAPACITY_ = 8;
    constexpr static const double CAPACITY_COEFF_ = 0.85;

    size_t arr_capacity_;

    std::list<Entry<T>> **arr_;
    std::list<T> history_;

    inline size_t get_hash_pos_(const T &e, size_t capacity) const {
        return Hasher()(e) % capacity;
    }

    void init_hashset(size_t capacity) {
        arr_ = new std::list<Entry<T>> *[capacity]();
        arr_capacity_ = capacity;

        std::list<T> list = history_;
        history_.clear();
        for (T e: list) {
            insert(e);
        }
    }

    inline void delete_arr_() {
        deep_delete_sub_arr_();
        delete[] arr_;
    }

    void deep_delete_sub_arr_() {
        if (this == nullptr)
            return;
        // Оптимизация по количеству элементов невозможна, т.к. мы не знаем, какие листы были инициализированы
        for (size_t i = 0; i < arr_capacity_; i++) {
            std::list<Entry<T>> *&cur_list = arr_[i];
            delete cur_list;
        }
    }
};

#endif //LINKEDHASHMAP_LINKEDHASHSET_H
