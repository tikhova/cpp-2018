#pragma once
#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>

struct big_integer {
	//Constructors
	big_integer();
	big_integer(big_integer const &second);
	big_integer(int const value);
	explicit big_integer(std::string const str);
	~big_integer();

	//Operators
	big_integer& operator=(big_integer const &second);

	friend bool operator<(const big_integer& left, const big_integer& right);
	friend bool operator>(const big_integer& left, const big_integer& right);
	friend bool operator<=(const big_integer& left, const big_integer& right);
	friend bool operator>=(const big_integer& left, const big_integer& right);
	friend bool operator==(const big_integer& left, const big_integer& right);

	friend const big_integer operator+(const big_integer& second);
	friend const big_integer operator-(const big_integer& second);
	friend const big_integer operator*(const big_integer& second);
	friend const big_integer operator/(const big_integer& left, const big_integer& right);
	friend const big_integer operator%(const big_integer& left, const big_integer& right);

	const bool operator~();
	friend const bool operator&(const big_integer& second);
	friend const bool operator|(const big_integer& left, const big_integer& right);
	friend const bool operator^(const big_integer& left, const big_integer& right);

	friend const big_integer operator>>(const big_integer& first, int x);
	friend const big_integer operator<<(const big_integer& first, int x);
};

std::string to_string(const big_integer& x);

#endif