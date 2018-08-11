#pragma once
#ifndef FREADER_H
#define FREADER_H

#include <string>
#include <fstream>
#include <vector>

class freader {
private:
    static constexpr size_t BUF_SIZE = 1024;
    static constexpr size_t CHAR_SIZE = 8;
    std::ifstream reader;
    unsigned char buffer[BUF_SIZE];
    size_t bit_index;
    size_t cur_index;
    size_t end_index;

    void read_chunk();

public:
    freader(std::string const &);
    ~freader();

    template <typename T>
    inline T get() { T x; reader >> x; return x; }

    bool get_bit();
    unsigned char get_char();
    std::vector<bool> get_bool_vector(size_t);
    std::vector<unsigned char> get_chunk();

    bool eof();
    void reset() { reader.clear(); reader.seekg(0); cur_index = end_index = bit_index = 0; }
    inline void skipws(bool state) { reader >> (state ? std::skipws : std::noskipws); }
};

#endif // FREADER_H
