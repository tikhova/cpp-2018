#pragma once
#ifndef COUNTER_H
#define COUNTER_H

#include <string> // size_t
#include <vector>
#include <map>

class counter {
private:
    static constexpr size_t SYMBOLS_AMOUNT = 256;
    std::vector<size_t> count;
    size_t LENGTH = 0;

public:
    counter();

    void add(unsigned char const & c);
    void add(std::vector<unsigned char> const &);

    std::multimap<size_t, size_t> get();
    inline size_t get_count(unsigned char const & c) { return count.at(c); }
    inline size_t get_total_amount() { return LENGTH; }
};

#endif // COUNTER_H
