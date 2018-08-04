#pragma once
#ifndef COUNTER_H
#define COUNTER_H

#include <string> // size_t
#include <vector>
#include <map>

class counter {
private:
    static constexpr size_t SYMBOLS_AMOUNT = 256;
    //uintmax_t count[SYMBOLS_AMOUNT];
    std::vector<size_t> count;
    //std::map<char, size_t> count;

public:
    counter();
    void add(char const c);
    void add(std::vector<char> const &);
    size_t get_count(char const);
    std::multimap<size_t, unsigned char> get();
};

#endif // COUNTER_H
