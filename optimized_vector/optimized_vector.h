#pragma once
#ifndef OPTIMIZED_VECTOR_H
#define OPTIMIZED_VECTOR_H

//#include <iostream>
//#include <cstdint>
#include <memory>


class optimized_vector {
public:
    using value_type = uint_least32_t;

private:
    static const size_t SMALL_SIZE = 16;
    size_t size_;
    value_type* data_ptr_;

    struct big_value{
        size_t capacity;
        std::shared_ptr<value_type> data;

        big_value(size_t, value_type*);
        big_value(big_value const &);
    };

    union value_union{
        value_type small[SMALL_SIZE];
        big_value big;

        value_union() {}
        ~value_union() {}
    } value_;

    // Utilits
    void make_unique();
    friend void swap_big_small(optimized_vector & big, optimized_vector & small);
    friend value_type* copy(big_value const &);
    friend value_type* copy(big_value const &, size_t);

public:
    // Construtors and destructor
    optimized_vector();
    optimized_vector(size_t, value_type);
    optimized_vector(optimized_vector const &) noexcept;
    ~optimized_vector() /*= default*/;

    // Operators
    optimized_vector & operator=(optimized_vector const &) noexcept;
    friend bool operator==(optimized_vector const &, optimized_vector const &);
    value_type & operator[](size_t);
    value_type const & operator[](size_t) const;

    // Access
    value_type const & back() const;
    value_type * begin();
    value_type * end();

    // Change
    void push_back(value_type const &);
    value_type pop_back();
    friend void swap(optimized_vector & x, optimized_vector & y);

    // Size
    void reserve(size_t const &);
    void resize(size_t, value_type);

    // Information
    inline size_t size() const { return size_; }
    size_t capacity() const;
    inline bool is_small() const { return size_ <= SMALL_SIZE; }
    inline bool is_big() const { return !is_small(); }
};

#endif // OPTIMIZED_VECTOR_H
