#pragma once
#ifndef buffered_reader_H
#define buffered_reader_H

#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>
#include <climits>

class buffered_reader {
private:
    static constexpr size_t BUFFER_SIZE = 1 << 18;
    std::istream & reader;
    char buffer[BUFFER_SIZE];
    size_t cur_index;
    size_t end_index;

    void read_chunk();

public:
    buffered_reader(std::istream &);

    template <typename T>
    bool get(T& result) {
        size_t const T_SIZE = sizeof(T);
        size_t buffer_size = end_index - cur_index;
        if (buffer_size >= T_SIZE) {
            std::memcpy(&result, buffer + cur_index, T_SIZE);
            cur_index += T_SIZE;
        } else {
            std::memcpy(&result, buffer + cur_index, buffer_size);
            read_chunk();
            if (end_index < T_SIZE - buffer_size) {
                return false;
            }
            std::memcpy(&result + buffer_size, buffer, T_SIZE - buffer_size);
            cur_index = T_SIZE - buffer_size;
        }
        return true;
    }

    void reset();
};

#endif // buffered_reader_H
