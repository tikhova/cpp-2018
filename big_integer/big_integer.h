#pragma once
#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>
#include <cstdint> // limits of int types
#include <limits>  // numeric_limits
#include <cmath>   // pow

class big_integer {
private:
        using digit_t = uint_least32_t;
        using double_digit_t = uint_least64_t;
        big_integer(digit_t const);

private:
        int sign;
        std::vector<digit_t> value;
        static constexpr int BITS_AMOUNT = std::numeric_limits<digit_t>::digits;
        static constexpr int DIGITS_AMOUNT = std::numeric_limits<digit_t>::digits10;
        static constexpr digit_t MAX_DIGIT = std::numeric_limits<digit_t>::max();
        static constexpr digit_t DIGITS_BASE = pow(10, DIGITS_AMOUNT);
        static constexpr double_digit_t BITS_BASE = (double_digit_t)1 << BITS_AMOUNT;

        // Utilits
        friend int compare_abs_value(big_integer const &, big_integer const &);
        friend std::string insert_leading_zeroes(const std::string &);
        friend void normalize(big_integer &);
        friend digit_t low(const double_digit_t &);
        friend digit_t high(const double_digit_t &);
        friend digit_t stodt(std::string);
        /// Arithmetic
        friend void add_with_carry(digit_t &, const digit_t &, digit_t &);
        friend void sub_with_carry(digit_t &, const digit_t &, digit_t &);
        friend void div_with_carry(digit_t &, const digit_t &, digit_t &);
        friend void add_value(big_integer &, big_integer const &, big_integer const &);
        friend void sub_value(big_integer &, big_integer const &, big_integer const &);
        friend void mul_value(big_integer &, big_integer const &, big_integer const &);
        friend void div_value_with_digit(big_integer&, big_integer const &, const digit_t&, digit_t&);
        big_integer& negate();
        //// Long division
        friend digit_t trial(big_integer const &, big_integer const &, const size_t&, const size_t&);
        friend bool smaller(big_integer const &, big_integer const &, const size_t&, const size_t&);
        friend std::pair<big_integer, big_integer> div_value(/*big_integer&, big_integer&, */big_integer const &, big_integer const &);
        //// Logic
        template <class F>
        static big_integer logic_operation(big_integer a, big_integer b, F lambda);

public:
        // Constructors
        big_integer();
        big_integer(big_integer const  &) = default;
        big_integer(int const);
        explicit big_integer(std::string const &);
        ~big_integer() = default;

        // Operators

        /// Logic operators
        big_integer& operator=(big_integer const &) = default;
        friend bool operator==(big_integer const &, big_integer const &);
        friend bool operator!=(big_integer const &, big_integer const &);
        friend bool operator<(big_integer const &, big_integer const &);
        friend bool operator>(big_integer const &, big_integer const &);
        friend bool operator<=(big_integer const &, big_integer const &);
        friend bool operator>=(big_integer const &, big_integer const &);

        /// Arithmetic operators
        big_integer& operator+=(big_integer const &);
        big_integer& operator-=(big_integer const &);
        big_integer& operator*=(big_integer const &);
        big_integer& operator/=(big_integer const &);
        big_integer& operator%=(big_integer const &);

        friend big_integer operator+(big_integer const &, big_integer const &);
        friend big_integer operator-(big_integer const &, big_integer const &);
        friend big_integer operator*(big_integer const &, big_integer const &);
        friend big_integer operator/(big_integer const &, big_integer const &);
        friend big_integer operator%(big_integer const &, big_integer const &);

        big_integer& operator++();
        big_integer& operator--();

        big_integer operator-() const;
        big_integer operator+() const;

        /// Binary operators
        big_integer& operator>>=(unsigned int);
        big_integer& operator<<=(unsigned int);

        friend big_integer operator>>(big_integer const &, unsigned int);
        friend big_integer operator<<(big_integer const &, unsigned int);

        big_integer& operator~();

        big_integer& operator&=(big_integer const &);
        big_integer& operator|=(big_integer const &);
        big_integer& operator^=(big_integer const &);

        friend big_integer operator&(big_integer const &, big_integer const &);
        friend big_integer operator|(big_integer const &, big_integer const &);
        friend big_integer operator^(big_integer const &, big_integer const &);

        // Global
        friend std::string to_string(big_integer const &);

        // Utilits
        friend big_integer abs(big_integer const &);
        friend big_integer max(big_integer const &, big_integer const &);
        friend big_integer min(big_integer const &, big_integer const &);
};

void swap(big_integer &, big_integer &);
std::string insert_leading_zeroes(std::string const &);
#endif
