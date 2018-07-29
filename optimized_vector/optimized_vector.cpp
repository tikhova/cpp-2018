#include "optimized_vector.h"
#include <limits> // numeric_limits
#include <string.h> // memcpy

using value_type = uint_least32_t;

// Constructors and destructor

optimized_vector::big_value::big_value(size_t capacity_, value_type* data_) : capacity(capacity_), data(data_) {}

optimized_vector::big_value::big_value(big_value const & x) : capacity(x.capacity), data(x.data) {}

optimized_vector::optimized_vector() : size_(0), data_ptr_(value_.small) {}

optimized_vector::optimized_vector(size_t n, value_type x) : size_(n) {
    if (is_small()) {
        data_ptr_ = value_.small;
    } else {
        new (&value_.big) big_value(size_, new value_type[size_]);
        data_ptr_ = value_.big.data.get();
    }
    for(size_t i = 0; i != size_; ++i) {
        (*this)[i] = x;
    }
}

optimized_vector::optimized_vector(optimized_vector const & x) noexcept : size_(x.size_) {
    if (is_small()) {
        data_ptr_ = value_.small;
        for(size_t i = 0; i != size_; ++i) {
            (*this)[i] = x[i];
        }
    } else {
        new (&value_.big) big_value(x.value_.big);
        data_ptr_ = value_.big.data.get();
    }
}

optimized_vector::~optimized_vector() {
    if (is_big()) {
        value_.big.~big_value();
    }
}

// Utilits

value_type* copy(optimized_vector::big_value const & x) {
    value_type * result = new value_type[x.capacity];
    value_type * x_ptr = x.data.get();
    for (size_t i = 0; i != x.capacity; ++i) {
        result[i] = x_ptr[i];
    }
    return result;
}

value_type* copy(optimized_vector::big_value const & x, size_t new_capacity) {
    value_type * result = new value_type[new_capacity];
    value_type * x_ptr = x.data.get();
    size_t copy_length = std::min(x.capacity, new_capacity);
    for (size_t i = 0; i != copy_length; ++i) {
        result[i] = x_ptr[i];
    }
    for (size_t i = copy_length; i != new_capacity; ++i) {
        result[i] = 0;
    }
    return result;
}

// Information

size_t optimized_vector::capacity() const {
    if (is_small()) return SMALL_SIZE;
    return value_.big.capacity;
}

// Size

void optimized_vector::reserve(size_t const & n) {
    if (capacity() >= n) return;
    if (is_small()) {
        value_type * temp = new value_type[n];
        for (size_t i = 0; i != size_; ++i) {
            temp[i] = value_.small[i];
        }
        for(size_t i = size_; i != n; ++i) {
            temp[i] = 0;
        }
        new (&value_.big) big_value(n, temp);
    } else {
        value_.big.data.reset(copy(value_.big, value_.big.capacity * 2));
        value_.big.capacity *= 2;
    }

    data_ptr_ = value_.big.data.get();
}

void optimized_vector::resize(size_t new_size, value_type elem) {
    *this = optimized_vector(new_size, elem);
}

//Operators

value_type & optimized_vector::operator[](size_t index) {
    make_unique();
    return data_ptr_[index];
}

value_type const & optimized_vector::operator[](size_t index) const {
    return data_ptr_[index];
}

bool operator==(optimized_vector const & x, optimized_vector const & y) {
    if (x.size_ != y.size_) return false;
    for (size_t i = 0; i != x.size_; ++i) {
        if (x[i] != y[i]) return false;
    }
    return true;
}

optimized_vector & optimized_vector::operator=(optimized_vector const & x) noexcept {
    if (this != &x) {
        make_unique();
        optimized_vector t(x);
        swap(*this, t);
    }
    return *this;
}

void optimized_vector::make_unique() {
    if (is_big() && !value_.big.data.unique()) {
        value_.big.data.reset(copy(value_.big));
        data_ptr_ = value_.big.data.get();
    }
}

void optimized_vector::push_back(value_type const & x) {
    make_unique();
    reserve(size_ + 1);
    (*this)[size_++] = x;
}

value_type optimized_vector::pop_back() {
    make_unique();
    value_type result = data_ptr_[--size_];
    if (size_ == SMALL_SIZE) {
        value_type * temp = copy(value_.big);
        value_.big.~big_value();
        new (&value_.small) value_type[optimized_vector::SMALL_SIZE];
        for (size_t i = 0; i != size_; ++i) {
            value_.small[i] = temp[i];
        }
        delete [] temp;
        data_ptr_ = value_.small;
    }
    return result;
}


value_type const & optimized_vector::back() const {
    return data_ptr_[size_ - 1];
}

value_type * optimized_vector::begin() {
    return data_ptr_;
}

value_type * optimized_vector::end() {
    return data_ptr_ + size_;
}

void swap(optimized_vector & x, optimized_vector & y) {
    if (x.is_small() && y.is_small()) {
        for (size_t i = 0; i != optimized_vector::SMALL_SIZE; ++i) {
            std::swap(x[i], y[i]);
        }
    } else {
        if (x.is_big() && y.is_big()) {
            std::swap(x.value_.big, y.value_.big);
            std::swap(x.data_ptr_, y.data_ptr_);
        } else if (x.is_big()) {
            swap_big_small(x, y);
        } else {
            swap_big_small(y, x);
        }        
    }
    std::swap(x.size_, y.size_);
}

void swap_big_small(optimized_vector & big, optimized_vector & small) {
    big.make_unique();

    value_type temp[optimized_vector::SMALL_SIZE];
    for (size_t i = 0; i != optimized_vector::SMALL_SIZE; ++i) {
        temp[i] = small[i];
    }

    new (&small.value_.big) optimized_vector::big_value(big.value_.big);
    small.data_ptr_ = small.value_.big.data.get();

    big.value_.big.~big_value();
    new (&big.value_.small) value_type[optimized_vector::SMALL_SIZE];
    for (size_t i = 0; i != optimized_vector::SMALL_SIZE; ++i) {
        big.value_.small[i] = temp[i];
    }
    big.data_ptr_ = big.value_.small;
}
