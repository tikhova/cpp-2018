#include "big_integer.h"
#include <stdexcept>    // overflow_error, invalid_argument
#include <numeric>      // accumulate
#include <algorithm>    // transform
#include <iterator>     // iterator, front_inserter, back_inserter
#include <cmath>        // pow
#include <functional>   // bit operations

using std::invalid_argument;    using std::reverse;             using std::bit_not;
using std::to_string;           using std::string;              using std::bit_and;
using std::vector;              using std::transform;           using std::bit_or;
using std::accumulate;          using std::overflow_error;      using std::bit_xor;
using std::pair;

using digit_t = uint_least32_t;
using double_digit_t = uint_least64_t;

digit_t stodt(string str);

// Constructors

/// Copy constructor is default

big_integer::big_integer() : sign(0), value(1, 0) {}

big_integer::big_integer(int const number) {
    if (number == 0) {
        sign = 0;
        value.push_back(0);
    } else {
        sign = number > 0 ? 1 : -1;
        value.push_back(std::abs(number));
    }
}

big_integer::big_integer(string const & str): big_integer() {
    size_t index = 0;
    size_t str_length = str.length();

    if (!isdigit(str[0])) {
        ++index;
    }

    size_t tail_size = (str_length - index) % DIGITS_AMOUNT; // push the front
    if (tail_size != 0) {                                    // of the number
        *this = stodt(str.substr(index, tail_size));
        index += tail_size;
    }

    while (index != str_length) {                           // push everything else
        *this = *this * DIGITS_BASE + stodt(str.substr(index, DIGITS_AMOUNT));
        index += DIGITS_AMOUNT;
    }

    if (str[0] == '-') {                                    // set sign = -1 if have previous minus
        sign = -1;
    }

    normalize(*this);
}

/// Private constructor
big_integer::big_integer(digit_t const number) {
    if (number == 0) {
        sign = 0;
        value.push_back(0);
    } else {
        sign = 1;
        value.push_back(number);
    }
}

// Operators

/// Logic operators
bool operator==(big_integer const & x, big_integer const & y) {
    return x.sign == y.sign && x.value == y.value;
}

bool operator!=(big_integer const & x, big_integer const & y) {
    return !(x == y);
}

bool operator<(big_integer const & x, big_integer const & y) {
    if (x.sign != y.sign) {                                // compare signs
        return x.sign < y.sign;
    } else {                                               // compare length
        return compare_abs_value(x, y) * x.sign == -1;
    }
}

bool operator>(big_integer const & x, big_integer const & y) {
    return y < x;
}

bool operator<=(big_integer const & x, big_integer const & y) {
    return !(x > y);
}

bool operator>=(big_integer const & x, big_integer const & y) {
    return !(x < y);
}

/// Arithmetic operators
big_integer& big_integer::operator+=(big_integer const & x) {
    if (sign == 0) {                         // if one operand is zero
        return *this = x;
    }
    if (x.sign == 0) {
        return *this;
    }

    if (sign != x.sign) {                   // if operands have different sign
        this->negate();              // this + (- x) = -((- this) - (- x)) = this - x
        *this -= x;                         // (- this) + x = -(this - x)
        return this->negate();
    }

    add_value(*this, *this, x);             // if operands have the same sign
    return *this;
}

big_integer& big_integer::operator-=(big_integer const & x) {
    if (sign == 0) {                         // if one of operands is zero
        return *this = x;
    }
    if (x.sign == 0) {
        return *this;
    }

    if (sign != x.sign) {                   // if operands have different sign
        this->negate();              // this - (- x) = -((- this) + (- x)) = this + x
        *this += x;                         // (- this) - x = -(this + x) = -this - x
        return this->negate();
    }

    sub_value(*this, *this, x);             // if operands have the same sign

    return *this;
}

big_integer& big_integer::operator*=(big_integer const & x) {
    this->sign = sign * x.sign;
    if (this->sign == 0) {                      // if one of the operands is zero
        this->value = vector<digit_t>(1, 0);
    } else {
        mul_value(*this, *this, x);
    }

    return *this;
}

big_integer& big_integer::operator/=(big_integer const & x) {
    *this = div_value(*this, x).first;
    this->sign *= x.sign;
    return *this;
}

big_integer& big_integer::operator%=(big_integer const & x) {
    *this = div_value(*this, x).second;
    return *this;
}


big_integer operator+(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result += y;
}

big_integer operator-(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result -= y;
}

big_integer operator*(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result *= y;
}

big_integer operator/(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result /= y;
}

big_integer operator%(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result %= y;
}


big_integer& big_integer::operator++() {
    return *this += 1;
}

big_integer& big_integer::operator--() {
    return *this -= 1;
}


big_integer big_integer::operator-() const {
    big_integer result = *this;
    return result.negate();
}

big_integer big_integer::operator+() const {
    return *this;
}

/// Binary operators
big_integer& big_integer::operator~() {
    this->negate();
    return --(*this);
}

big_integer& big_integer::operator>>=(unsigned int n) {
    if (n == 0) {
        return *this;
    }

    if (*this < 0) {                     // inverted two's complement
        *this -= 1;
    }

    vector<digit_t> result;
    size_t full_blocks = 0;
    unsigned int shift = n;

    if (n >= BITS_AMOUNT) {
        full_blocks = n / BITS_AMOUNT;  // amount of full blocks to be shifted
        shift = n % BITS_AMOUNT;        // amount of bits to be shifted
    }    

    if(full_blocks >= value.size()) {
        *this = 0;
        return *this;
    }

    digit_t shift_mask = ((digit_t)1 << shift) - 1; // ((1 << n) - 1) - n times 1
    digit_t carry = 0;
    digit_t d; // current digit
    size_t size = value.size();
    for (size_t i = size - 1; i >= full_blocks && i < size; --i) {
        d = value[i] >> shift;                // value[i] high (BITS_AMOUNT - shift) digits
        carry <<= (BITS_AMOUNT - shift);
        d |= carry;                           // add carry from previous digit
        carry = value[i] & shift_mask;        // value[i] low shift digits
        result.push_back(d);
    }
    reverse(result.begin(), result.end());
    this->value = result;

    if (*this < 0) {        // back to one's complement
        *this -= 1;
    }

    normalize(*this);
    return *this;
}

big_integer& big_integer::operator<<=(unsigned int n) {
    if (n == 0) {
        return *this;
    }

    vector<digit_t> result;
    size_t full_blocks = 0;
    unsigned int shift = n;

    if (n >= BITS_AMOUNT) {
        full_blocks = n / BITS_AMOUNT;  // amount of full blocks to be shifted
        shift = n % BITS_AMOUNT;        // amount of bits to be shifted
    }


    double_digit_t rest_mask =
            ((double_digit_t)1 << (BITS_AMOUNT - shift)) - 1; // ((1 << n) - 1) - n times 1
    digit_t carry = 0;
    digit_t d = 0; // current digit
    size_t size = value.size();

    for (size_t i = 0; i != full_blocks; ++i) { // insert end zeroes
        result.push_back(0);
    }

    for (size_t i = 0; i < size; ++i) {
        d = value[i] & rest_mask;                  // value[i] low (BITS_AMOUNT - shift) digits
        d = (d << shift) | carry;                  // add carry from previous digit
        carry = ((double_digit_t)value[i]) >>      // value[i] high shift digits
                    (BITS_AMOUNT - shift);
        result.push_back(d);
    }
    this->value = result;

    return *this;
}

big_integer operator>>(big_integer const & x, unsigned int n) {
    big_integer result = x;
    return result >>= n;
}

big_integer operator<<(big_integer const & x, unsigned int n) {
    big_integer result = x;
    return result <<= n;
}


big_integer& big_integer::operator&=(big_integer const & x) {
    return *this = this->logic_operation(*this, x, bit_and<digit_t>());
}

big_integer& big_integer::operator|=(big_integer const & x) {
    return *this = this->logic_operation(*this, x, bit_or<digit_t>());
}

big_integer& big_integer::operator^=(big_integer const & x) {
    return *this = this->logic_operation(*this, x, bit_xor<digit_t>());
}


big_integer operator&(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result &= y;
}

big_integer operator|(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result |= y;
}

big_integer operator^(big_integer const & x, big_integer const & y) {
    big_integer result = x;
    return result ^= y;
}

// Global

string to_string(big_integer const & bi) {
    if(bi == 0) {
        return "0";
    }

    string sign = (bi.sign == -1) ? "-" : "";
    vector<string> pieces;
    big_integer x = bi;
    big_integer r;
    while(x != 0) {
        r = x % big_integer::DIGITS_BASE;
        pieces.push_back(to_string(r.value.back()));
        x /= big_integer::DIGITS_BASE;
    }
    vector<string> normalized_pieces;
    normalized_pieces.push_back(pieces.back());
    transform(pieces.rbegin() + 1, pieces.rend(), // insert leading zeroes to digits
              back_inserter(normalized_pieces), insert_leading_zeroes);
    return sign + accumulate(normalized_pieces.begin(), normalized_pieces.end(),
                             string(), std::plus<string>());
}

// Utilits

/// Private

// x < y - (-1);   x == y - 0;  x > y - 1;
int compare_abs_value(big_integer const & x, big_integer const & y) {
    size_t x_size = x.value.size();
    size_t y_size = y.value.size();
    if (x_size < y_size) {          // compare length
        return -1;
    } else if (x_size > y_size){
        return 1;
    } else {                        // compare values
        for (size_t i = x_size - 1; i < x_size; --i) {
            if (x.value[i] != y.value[i]) {
                return (x.value[i] < y.value[i]) ? -1 : 1;
            }
        }
        return 0;
    }
}

string insert_leading_zeroes(string const & s) {
    string res;
    int n = big_integer::DIGITS_AMOUNT - s.length();
    if (n > 0) {
        res = string(n, '0');
        res.append(s);
    }
    return res;
}

void normalize(big_integer & x) {
    while (x.value.size() > 1 && x.value.back() == 0) { // delete front zeroes
        x.value.pop_back();
    }

    if (x.value.size() == 1 && x.value.back() == 0) { // if x == 0
        x.sign = 0;
    }
}

digit_t low(double_digit_t const & x) {
    return x & big_integer::MAX_DIGIT;
}

digit_t high(double_digit_t const & x) {
    return x >> big_integer::BITS_AMOUNT;
}

// converts string to digit_t
digit_t stodt(string str) {
    digit_t res = 0;
    digit_t multiplyer = 1;
    for (size_t i = str.size(); i-- != 0; multiplyer *= 10) {
        res += multiplyer * (str[i] - '0');
    }
    return res;
}

//// Arithmetic

// summates x, y and carry
// puts the result to x, new carry to carry
void add_with_carry(digit_t& x, digit_t const & y, digit_t& carry) {
    double_digit_t sum = (double_digit_t)x + (double_digit_t)y + (double_digit_t)carry;
    x = low(sum);
    carry = high(sum);
}

// subtracts y and carry from x
// puts the result to x, new carry to carry
void sub_with_carry(digit_t& x, digit_t const & y, digit_t& carry) {
    double_digit_t t = x;
    double_digit_t sub = y + carry;
    if (sub > t) {
        t += big_integer::BITS_BASE;
        carry = 1;
    } else {
        carry = 0;
    }
    x = (digit_t)(t - sub);
}

// divides x by y
// puts the result to x, remainder to carry
void div_with_carry(digit_t& x, digit_t const & y, digit_t& carry) {
    double_digit_t temp = carry * big_integer::BITS_BASE + x;
    x = temp / y;
    carry = temp % y;
}

void add_value(big_integer& result, big_integer const & x, big_integer const & y) {
    big_integer longer = x;
    big_integer shorter = y;
    if (longer < shorter) {
        swap(longer, shorter);
    }
    digit_t carry = 0;
    size_t i;
    size_t longer_size = longer.value.size();
    size_t shorter_size = shorter.value.size();
    for (i = -1; ++i != shorter_size;
         add_with_carry(longer.value[i], shorter.value[i],  carry));

    while (carry != 0) {
        if (i != longer_size) {
            add_with_carry(longer.value[i], 0,  carry);
            ++i;
        } else {
            longer.value.push_back(carry);
            carry = 0;
        }
    }
    result = longer;
}

void sub_value(big_integer& result, big_integer const & x, big_integer const & y) {
    bool neg = false;
    big_integer longer = x, shorter = y;
    if (compare_abs_value(longer, shorter) == -1) {
        swap(longer, shorter);
        neg = true;
    }

    digit_t carry = 0;
    size_t i;
    size_t longer_size = longer.value.size();
    size_t shorter_size = shorter.value.size();
    for (i = -1; ++i != shorter_size;
         sub_with_carry(longer.value[i], shorter.value[i], carry));

    while (carry != 0) {
        if (i != longer_size) {
            sub_with_carry(longer.value[i], 0, carry);
            ++i;
        }
    }
    normalize(longer);
    result = longer;
    if(neg) {
        result.negate();
    }
}

void mul_value(big_integer& result, big_integer const & x, big_integer const & y) {
    big_integer res = 0;
    res.sign = x.sign;
    res.value.resize(x.value.size() + y.value.size() + 2, 0);
    double_digit_t d;
    digit_t carry = 0;
    size_t n = x.value.size();
    size_t m = y.value.size();

    for (size_t i = 0; i != n; ++i) {
        carry = 0;
        for (size_t j = 0; j != m; ++j) {
            d = (double_digit_t)x.value[i] * y.value[j] + carry + res.value[i + j];
            res.value[i + j] = low(d);
            carry = high(d);
        }
        res.value[i + m] += carry;
    }
    normalize(res);
    result = res;
}

void div_value_with_digit(big_integer& result, big_integer const & x, digit_t const & y, digit_t& carry) {
    digit_t c = 0;
    size_t x_length = x.value.size();
    result = x;
    for (size_t i = x_length; i-- != 0; div_with_carry(result.value[i], y, c));
    carry = c;
}

///// Long division
// computation of a trial value of k-th quotent digit
digit_t trial(big_integer const & r, big_integer const & d, size_t const & k, size_t const & m) {
    if (r == 0) {
        return 0;
    }
    size_t km = k + m;
    double_digit_t r2 = ((double_digit_t)r.value[km] << big_integer::BITS_AMOUNT) |
                        r.value[km - 1]; // r{2}
    double_digit_t d1 = d.value[m - 1]; // d{1}
    return std::min((digit_t)(r2 / d1), (digit_t)big_integer::MAX_DIGIT);
}

// r{m + 1} < dq ?
bool smaller(big_integer const & r, big_integer const & dq, size_t const & k, size_t const & m) {
    size_t i = m;
    size_t j = 0;
    while (i != j) {
        if (r.value[i + k] != dq.value[i]) {
            j = i;
        } else {
            --i;
        }
    }
    return r.value[i + k] < dq.value[i];
}

// first argument is result, second is remainder
pair<big_integer, big_integer> div_value(big_integer const & x, big_integer const & y) {
    if (compare_abs_value(x, y) == -1) { // if x is smaller than y
        return pair<big_integer, big_integer>{0, x};
    }

    big_integer result;
    big_integer remainder;
    if (y.value.size() == 1) { // if y is a digit use simpler division algo
        digit_t r = 0;
        int sign = x.sign;
        div_value_with_digit(result, x, y.value.back(), r);
        remainder = r;
        if (sign == -1) {
            remainder.negate();
        }
    } else {
        digit_t scaling_factor = big_integer::BITS_BASE / (y.value.back() + 1);
        big_integer r = x.sign * x * scaling_factor; // scaling x (remainder)
        big_integer d = y.sign * y * scaling_factor; // scaling y (divider)
        big_integer q;                               // quotient quotient
        digit_t qt;                                  // quotient trial digit
        size_t n = r.value.size();                   // n - size of remainder
        size_t m = d.value.size();                   // m - size of divider
        big_integer dq;

        q.sign = x.sign;
        r.value.push_back(0); // TO DO
        for (size_t k = n - m; k < n - m + 1; --k) {
            qt = trial(r, d, k, m);
            if (qt == 0) {
                q.value.push_back(qt);     // insert zero
                continue;
            }
            dq = d * qt;
            dq.value.push_back(0); // TO DO
            while (smaller(r, dq, k, m)) { // if qt is too big (r < dq * d * base ^ k)
                qt--;                      // decrease qt
                dq -= d;                   // fix dq
            }
            q.value.push_back(qt);         // insert digit
            normalize(dq);                 // for correct subtraction
            r -= (dq << (big_integer::BITS_AMOUNT * k)); // update remainder
        }
        remainder = r / scaling_factor;    // scaling remainder
        reverse(q.value.begin(), q.value.end()); // reverse for correct representation
        result = q;
    }
    normalize(result);
    return pair<big_integer, big_integer>{result, remainder};
}

//// Logic
template <class F> // TO DO: Fix strange transform(?) usages
big_integer big_integer::logic_operation(big_integer a, big_integer b, F lambda) { // TO DO: comment the shit out of it
    vector<digit_t> temp;

    if (a.sign == -1) {
        transform(a.value.begin(), a.value.end(), back_inserter(temp), bit_not<digit_t>());
        a.value = temp;
        temp = vector<digit_t>();
        a.sign = 1;
        a += 1;
    } else {
        a.sign = 0;
    }

    if (b.sign == -1) {
        transform(b.value.begin(), b.value.end(), back_inserter(temp), bit_not<digit_t>());
        b.value = temp;
        temp = vector<digit_t>();
        b.sign = 1;
        b += 1;
    } else {
        b.sign = 0;
    }

    if (a.value.size() < b.value.size()) {
        a.value.resize(b.value.size(), 0);
    } else if (a.value.size() > b.value.size()) {
        b.value.resize(a.value.size(), 0);
    }

    a.sign = lambda(a.sign, b.sign);

    for (size_t i = 0; i < a.value.size(); ++i) {
        a.value[i] = lambda(a.value[i], b.value[i]);
    }

    if(a.sign == 1) {
        transform(a.value.begin(), a.value.end(), back_inserter(temp), bit_not<digit_t>());
        a.value = temp;
        temp = vector<digit_t>();
        a += 1;
        a.sign = -1;
    } else {
        a.sign = 1;
    }

    normalize(a);

    if(a.value.back() == 0) {
        a.sign = 0;
    }

    return a;
}

/// Public

big_integer& big_integer::negate() {
    this->sign *= -1;
    return *this;
}

big_integer abs(big_integer const & x) {
    big_integer result = x;
    result.sign *= result.sign;
    return result;
}

big_integer max(big_integer const & x, big_integer const & y) {
    return x > y ? x : y;
}

big_integer min(big_integer const & x, big_integer const & y) {
    return x < y ? x : y;
}

void swap(big_integer & x, big_integer & y) {
    big_integer t = x;
    x = y;
    y = t;
}

