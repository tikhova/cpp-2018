#pragma once
#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>

// TO DO: Optimize for cycles in add_value, sub_value,
//        check if I can optimize any other cycle
//        check if I substituted front-to-end storage of value to end-to-front everywhere, where needed

typedef uint_least32_t digit_t;         // Q: Is there better way to do this?
typedef uint_least64_t double_digit_t;

class big_integer {
private:
        int sign;
        std::vector<digit_t> value;
        static constexpr int BITS_AMOUNT = 32;              // TO DO: Make <- this more decent
        static constexpr int DIGITS_AMOUNT = 9;
        static constexpr auto MAX_DIGIT = 999999999;
        static const double_digit_t BASE = MAX_DIGIT + 1;

        big_integer& value_op(big_integer&, const big_integer&);
        friend int compare_abs_value(const big_integer&, const big_integer&);
        friend void add_with_carry(digit_t&, const digit_t&, digit_t&);
        friend void sub_with_carry(digit_t&, const digit_t&, digit_t&);
        friend void mul_with_carry(digit_t&, const digit_t&, digit_t&);
        friend void add_value(big_integer&, const big_integer&, const big_integer&);
        friend void sub_value(big_integer&, const big_integer&, const big_integer&);
        friend void mul_value(big_integer&, const big_integer&, const big_integer&);
        friend std::string insert_leading_zeroes(const std::string&);
        friend void normalize(big_integer&);
        friend digit_t low(const double_digit_t&);
        friend digit_t high(const double_digit_t&);

public:
        //Constructors
        big_integer();
        big_integer(const big_integer&) = default;
        big_integer(const int);
        explicit big_integer(const std::string&);
        ~big_integer() = default;

        //Operators
        big_integer& operator=(const big_integer&) = default;
        friend bool operator==(const big_integer&, const big_integer&);
        friend bool operator!=(const big_integer&, const big_integer&);
        friend bool operator<(const big_integer&, const big_integer&);
        friend bool operator>(const big_integer&, const big_integer&);
        friend bool operator<=(const big_integer&, const big_integer&);
        friend bool operator>=(const big_integer&, const big_integer&);

        big_integer& operator+=(const big_integer&);
        big_integer& operator-=(const big_integer&);
        big_integer& operator*=(const big_integer&);


        friend big_integer operator+(const big_integer&, const big_integer&);
        friend big_integer operator-(const big_integer&, const big_integer&);

        /*
        friend const big_integer operator*(const big_integer&, const big_integer&);
        friend const big_integer operator/(const big_integer&, const big_integer&);
        friend const big_integer operator%(const big_integer&, const big_integer&);

        const bool operator~();
        friend const bool operator&(const big_integer&);
        friend const bool operator|(const big_integer&, const big_integer&);
        friend const bool operator^(const big_integer&, const big_integer&);

        friend const big_integer operator>>(const big_integer&, int);
        friend const big_integer operator<<(const big_integer&, int);
        */
        friend big_integer negate(const big_integer&);
        friend std::string to_string(const big_integer&);

};

digit_t stodt(std::string);
void swap(big_integer&, big_integer&);

#endif
