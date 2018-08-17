#pragma once
#ifndef FREADER_H
#define FREADER_H

#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>
#include <climits>

class freader {
private:
    static constexpr size_t BUFFER_SIZE = 1024 * 1024;
    static constexpr size_t CHAR_SIZE = CHAR_BIT;
    std::ifstream reader;
    unsigned char buffer[BUFFER_SIZE];
    size_t bit_index;
    size_t cur_index;
    size_t end_index;

    void read_chunk();

public:
    freader(std::string const &);
    ~freader();

    template <typename T>
    T get() {
        T result;
        size_t const T_SIZE = sizeof(T);
        size_t buffer_size = end_index - cur_index;
        if (buffer_size >= T_SIZE) {
            std::memcpy(&result, buffer + cur_index, T_SIZE);
            cur_index += T_SIZE;
        } else {
            std::memcpy(&result, buffer + cur_index, buffer_size);
            read_chunk();
            if (end_index < T_SIZE - buffer_size) {
                throw std::runtime_error("could not read from source");
            }
            std::memcpy(&result + buffer_size, buffer, T_SIZE - buffer_size);
            cur_index += T_SIZE - buffer_size;
        }
        return result;
    }

    bool get_bit();
    std::vector<bool> get_bool_vector(size_t);
    std::vector<unsigned char> get_chunk();

    void reset();
    inline bool eof() { return reader.eof() && (cur_index == end_index); }
    inline void skipws(bool state) { reader >> (state ? std::skipws : std::noskipws); }
};

#endif // FREADER_H
