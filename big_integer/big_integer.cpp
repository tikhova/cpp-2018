#include "big_integer.h"
#include <stdexcept>    // overflow_error, invalid_argument
#include <numeric>      // accumulate
#include <algorithm>    // transform
#include <iterator>     // iterator, front_inserter, back_inserter
#include <cmath>        // pow

using std::abs;
using std::to_string;
using std::vector;
using std::reverse;
using std::string;
using std::transform;
using std::accumulate;
using std::overflow_error;
using std::invalid_argument;

// TO DO: create a different library for some of the utilits, add others to big_integer
// Utilits names
big_integer abs(const big_integer&);
//big_integer& add_value (big_integer&, big_integer&);
big_integer max(const big_integer&, const big_integer&);
big_integer min(const big_integer&, const big_integer&);

// Constructors
big_integer::big_integer() : sign(0), value(1, 0) {}

big_integer::big_integer(const int number) {
    if (number == 0) {
        sign = 0;
        value.push_back(0);
    } else {
        sign = number > 0 ? 1 : -1;
        unsigned int absolute_number = abs(number);

        if (std::numeric_limits<unsigned int>::digits <= BITS_AMOUNT) {
            value.push_back(absolute_number);
        } else {
            while (absolute_number) {
                value.push_back(absolute_number && ((1 << BITS_AMOUNT) - 1));
                absolute_number <<= BITS_AMOUNT;
            }
        }
    }
    reverse(value.begin(), value.end());
}

big_integer::big_integer(const string& str) {
    string::size_type index = 0;
    string::size_type str_length = str.length();

    if (!isdigit(str[0])) {             // set sign = -1 if number < 0
        if(str[0] == '-') {             // set sign = 1 if number >= 0
            sign = -1;
        } else if(str[0] == '+') {
            sign = 1;
        }
        ++index;
    } else {
        sign = 1;
    }

    string::size_type tail_size = (str_length - index) % DIGITS_AMOUNT; // push the front
    if (tail_size != 0) {                                               // of the number
        value.push_back(stodt(str.substr(index, tail_size)));
        index += tail_size;
    }

    while (index != str_length) {                                       // push everything else
        digit_t x = stodt(str.substr(index, DIGITS_AMOUNT));
        int i = to_string(x).length();
        if(i < DIGITS_AMOUNT) {
            for(; i < DIGITS_AMOUNT; ++i, value.push_back(0));
        }
        value.push_back(x);
        index += DIGITS_AMOUNT;
    }

    if (value.size() == 1 && value.at(0) == 0) { // set sign = 0 if number == 0
        sign = 0;
    }
    reverse(value.begin(), value.end());
}

// Operators

bool operator==(const big_integer& x, const big_integer& y) {
    return x.sign == y.sign && x.value == y.value;
}

bool operator!=(const big_integer& x, const big_integer& y) {
    return !(x == y);
}

bool operator<(const big_integer& x, const big_integer& y) {
    if (x.sign != y.sign) {                                // compare signs
        return x.sign < y.sign;
    } else {                                                        // compare length
        return compare_abs_value(x, y) == -1;
    }
}

bool operator>(const big_integer& x, const big_integer& y) {
    return y < x;
}

bool operator<=(const big_integer& x, const big_integer& y) {
    return !(x > y);
}

bool operator>=(const big_integer& x, const big_integer& y) {
    return !(x < y);
}

big_integer& big_integer::operator+=(const big_integer& x) {
    if (sign == 0) {                         // if one operand is zero
        return *this = x;
    }
    if (x.sign == 0) {
        return *this;
    }

    if (sign != x.sign) {                   // if operands have different sign
        *this = negate(*this);
        *this -= x;
        return *this = negate(*this);
    }

    add_value(*this, *this, x);

    return *this;
}

big_integer& big_integer::operator-=(const big_integer& x) {
    if (sign == 0) {                         // if one of operands is zero
        return *this = x;
    }
    if (x.sign == 0) {
        return *this;
    }

    if (sign != x.sign) {                   // if operands have different sign
        *this = negate(*this);
        *this += x;
        return *this = negate(*this);
    }

    sub_value(*this, *this, x);                   // both operands are positive

    return *this;
}

big_integer& big_integer::operator*=(const big_integer& x) {
    int new_sign = sign * x.sign;
    this->sign = new_sign;
    if(new_sign == 0) {
        this->value = vector<digit_t>(1, 0);
    } else {
        mul_value(*this, *this, x);
    }

    return *this;
}

big_integer operator+(const big_integer& x, const big_integer& y) {
    big_integer result = x;
    return result += y;
}

big_integer operator-(const big_integer& x, const big_integer& y) {
    big_integer result = x;
    return result -= y;
}

// Global

string insert_leading_zeroes(const string& s) {
    return string(big_integer::DIGITS_AMOUNT - s.length(), 0).append(s);
}

string to_string(const big_integer& bi) {
    string sign = bi.sign == -1 ? "-" : "";
    vector<string> pieces;
    transform(bi.value.begin(), bi.value.end(), back_inserter(pieces), // convert each element to string
              (string (*)(uint_fast32_t))to_string);
    vector<string> normalized_pieces;
    normalized_pieces.push_back(pieces.back());
    transform(pieces.begin(), pieces.end() - 1, back_inserter(normalized_pieces), insert_leading_zeroes);

    return sign + accumulate(pieces.rbegin(), pieces.rend(), string(), std::plus<string>()); // put the strings together
}

// Utilits

int compare_abs_value(const big_integer& x, const big_integer& y) {
    vector<digit_t>::size_type x_size = x.value.size();
    vector<digit_t>::size_type y_size = y.value.size();
    if (x_size < y_size) {          // compare length
        return -1;
    } else if (x_size > y_size){
        return 1;
    } else {                        // compare values
        bool not_equal = false;
        for (vector<uint_fast32_t>::size_type i = x_size; i-- != 0; ) {
            if (x.value[i] < y.value[i]) {
                return -1;
            }
            if (x.value[i] != y.value[i]) {
                not_equal = true;
            }
        }
        if(not_equal) {
            return 1;
        }
        return 0;
    }
}

/// converts string to uint_fast32_t number
digit_t stodt(string str) {
    string::size_type length = str.size();

    if (length > std::numeric_limits<digit_t>::digits10) {
        throw overflow_error("data_t cannot contain" + str);
    }

    digit_t res = 0;
    digit_t multiplyer = 1;
    for (string::size_type i = length; i-- != 0; multiplyer *= 10) {
        if (!isdigit(str[i])) {
            throw invalid_argument(str + " is not an unsigned number");
        }
        res += multiplyer * (str[i] - '0');
    }
    return res;
}

big_integer abs(const big_integer& x) {
    negate(x);
    if(x < 0) {
        return negate(x);
    }
    return x;
}

big_integer negate(const big_integer& x) {
    big_integer res = x;
    res.sign = -res.sign;
    return res;
}

/// summates x, y and carry
/// puts the result to x, new carry to carry
void add_with_carry(digit_t& x, const digit_t& y, digit_t& carry) {
    double_digit_t sum = x + y + carry;
    x = sum % big_integer::MAX_DIGIT;
    carry = sum > x ? 1 : 0;
}

/// subtracts y and carry from x
/// puts the result to x, new carry to carry
void sub_with_carry(digit_t& x, const digit_t& y, digit_t& carry) {
    uint64_t sub = y + carry;
    if (sub > x) {
        x += big_integer::BASE;
        carry = 1;
    } else {
        carry = 0;
    }
    x -= sub;
}

void mul_with_carry(digit_t& x, const digit_t& y, digit_t& carry) {
    digit_t a = x >> big_integer::BITS_AMOUNT/2;
    digit_t b = x && (std::pow(2, big_integer::BITS_AMOUNT) - 1);
    digit_t c = y >> big_integer::BITS_AMOUNT/2;
    digit_t d = y && (std::pow(2, big_integer::BITS_AMOUNT) - 1);

    digit_t high = a*c;
    digit_t low = b*d;
    digit_t middle = (a + b)*(c + d) - (high + low);
    low += carry;
    middle += low >> big_integer::BITS_AMOUNT/2;
    low = low && (std::pow(2, big_integer::BITS_AMOUNT) - 1);
    high += middle >> big_integer::BITS_AMOUNT/2;
    middle = middle && (std::pow(2, big_integer::BITS_AMOUNT) - 1);
    carry = high;
    x = (middle << (big_integer::BITS_AMOUNT/2)) + low;
}

big_integer max(const big_integer& x, const big_integer& y) {
    return x > y ? x : y;
}

big_integer min(const big_integer& x, const big_integer& y) {
    return x < y ? x : y;
}

void add_value(big_integer& result, const big_integer& x, const big_integer& y) {
    big_integer longer = x, shorter = y;
    if (longer < shorter) {
        swap(longer, shorter);
    }
    digit_t carry = 0;
    vector<digit_t>::size_type longer_i, shorter_i;
    vector<digit_t>::size_type longer_size = longer.value.size();
    vector<digit_t>::size_type shorter_size = shorter.value.size();
    for (longer_i = longer_size, shorter_i = shorter_size;
         longer_i-- != 0 && shorter_i-- != 0;
         add_with_carry(longer.value[longer_size - longer_i - 1], shorter.value[shorter_size - shorter_i - 1], carry));

    while (carry != 0) {
        if (longer_i-- != 0) {
            add_with_carry(longer.value[longer_size - longer_i - 1], 0, carry);
        } else {
            longer.value.push_back(carry);
            carry = 0;
        }
    }
    result = longer;
}

void sub_value(big_integer& result, const big_integer& x, const big_integer& y) {
    bool neg = false;
    big_integer longer = x, shorter = y;
    if (compare_abs_value(longer, shorter) == -1) {
        swap(longer, shorter);
        neg = true;
    }


    digit_t carry = 0;
    vector<digit_t>::size_type longer_i, shorter_i;
    vector<digit_t>::size_type longer_size = longer.value.size();
    vector<digit_t>::size_type shorter_size = shorter.value.size();
    for (longer_i = longer.value.size(), shorter_i = shorter.value.size();
         longer_i-- != 0 && shorter_i-- != 0;
         sub_with_carry(longer.value[longer_size - longer_i - 1], shorter.value[shorter_size - shorter_i - 1], carry));

    while (carry != 0) {
        if (longer_i-- != 0) {
            sub_with_carry(longer.value[longer_size - longer_i - 1], 0, carry);
        } else {
            longer.value.push_back(9);
            neg = !(neg && neg);
            carry = 0;
        }
    }
    normalize(longer);
    result = longer;
    if(neg) {
        result = negate(result);
    }
}

//void mul_value(big_integer& result, const big_integer& x, const big_integer& y) {
//    big_integer longer = x, shorter = y;
//    if (longer < shorter) {
//        swap(longer, shorter);
//    }
//    vector<digit_t>::size_type shorter_i;
//    vector<digit_t>::size_type shorter_size = shorter.value.size();
//    big_integer res = 0;

//    big_integer t;
//    for (shorter_i = 0; shorter_i != shorter_size; ++shorter_i) {
//        t = longer;
//        mul_bi_by_digit(longer, shorter.value[shorter_i]);
//        for(int i = 0; i < shorter_i; ++i) {
//            t.value.push_back(0);
//        }
//        res += t;
//    }

//    result = res;
//}

digit_t low(const double_digit_t& x) {
    return x & ((1 << (big_integer::BITS_AMOUNT)) - 1);
}

digit_t high(const double_digit_t& x) {
    return x >> (big_integer::BITS_AMOUNT);
}

void mul_value(big_integer& result, const big_integer& x, const big_integer& y) {
    big_integer res;
    res.sign = x.sign;
    res.value.resize(x.value.size() + y.value.size() + 2, 0);
    double_digit_t d;
    digit_t carry = 0;
    big_integer a = x;
    big_integer b = y;

    for(int i = 0; i < a.value.size(); i++){
        carry = 0;
        for(int j = 0; j < b.value.size(); j++){
            d = (a.value[i] * b.value[j]);
            d += carry + res.value[i + j];
            res.value[i + j] = low(d);
            carry = high(d);
        }
        res.value[i + b.value.size()] += carry;
    }
    normalize(res);
    result = res;
}

void swap(big_integer& x, big_integer& y) {
    big_integer t = x;
    x = y;
    y = t;
}

void normalize(big_integer& x) {                            // delete all front zeros
    while (x.value.size() > 1 && x.value.back() == 0) {
        x.value.pop_back();
    }
}

