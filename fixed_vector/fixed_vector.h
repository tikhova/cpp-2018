#ifndef FIXED_VECTOR_H
#define FIXED_VECTOR_H

#include <type_traits> // aligned_storage
#include <algorithm> // copy_n
#include <cassert>
#include <iterator>

template <typename T, std::size_t N>
class fixed_vector {
public:
    typedef T* iterator ;
    typedef T const * const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Constructors & Destructor
    fixed_vector() : size_(0) {}

    fixed_vector(fixed_vector const & other) {
        copy_n(other.data, other.size_, this->data);
        size_ = other.size_;
    }

    ~fixed_vector() {
        std::for_each(begin(), end(), [](T& it){ it.~T();});
    }

    fixed_vector& operator=(fixed_vector const & other) {
        fixed_vector tmp(other);
        swap(*this, tmp);
        return *this;
    }

    // Information
    constexpr std::size_t capacity() const {
        return N;
    }

    std::size_t size() const {
        return size_;
    }

    bool empty() const {
        return !size_;
    }

    // Access
    T& operator[](std::size_t index) {
        return *reinterpret_cast<T*>(data + index);
    }

    T const & operator[](std::size_t index) const {
        return *reinterpret_cast<T const *>(data + index);
    }

    T& back() {
        return operator[](size_ - 1);
    }

    T const & back() const {
        return operator[](size_ - 1);
    }

    T& front() {
        return operator[](0);
    }

    T const & front() const {
        return operator[](0);
    }

    // Iterators
    T* begin() {
        return reinterpret_cast<T*>(data);
    }

    T const * begin() const {
        return reinterpret_cast<T const *>(data);
    }

    T* end() {
        return reinterpret_cast<T*>(data + size_);
    }

    T const * end() const {
        return reinterpret_cast<T const *>(data + size_);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    // Modification

    void push_back(T const & value) {
        new (data + size_++) T(value);
    }

    void pop_back() {
        back().~T();
        --size_;
    }

    void clear() {
        erase(begin(), end());
    }

    iterator erase(const_iterator it) {
        assert(it >= this->begin() && it < this->end());
        iterator i = const_cast<iterator>(it);
        std::rotate(i, i + 1, end());
        --size_;
        return i;
    }

    iterator erase(const_iterator begin, const_iterator end) {
        assert(begin >= this->begin() && end <= this->end());
        iterator first = const_cast<iterator>(begin);
        iterator second = const_cast<iterator>(end);
        std::rotate(first, second, this->end());
        size_ -= second - first;
        return first;
    }

    iterator insert(const_iterator iter, T const & val) {
        assert(size_ + 1 <= N);
        iterator it = const_cast<iterator>(iter);
        push_back(val);
        std::rotate(it, std::max(it, end() - 1), end());
        return static_cast<iterator>(it);
    }

    friend void swap(fixed_vector a, fixed_vector b) {
        std::swap(a, b);
    }

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
    std::size_t size_;
};

#endif // FIXED_VECTOR_H
