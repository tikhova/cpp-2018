#ifndef FWRITER_H
#define FWRITER_H

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <climits>

class  fwriter {
private:
    static constexpr size_t BUFFER_SIZE = 1024 * 1024;
    static constexpr size_t CHAR_SIZE = CHAR_BIT;
    unsigned char buffer[BUFFER_SIZE];
    std::ofstream writer;
    unsigned char current;
    size_t bit_index;
    size_t end_index;

public:
    fwriter(std::string const &filename);
    ~fwriter();

    void write_chunk();

    template <typename T>
    void put(T const & x) {
        size_t const T_SIZE = sizeof(x);
        size_t buffer_size = BUFFER_SIZE - end_index;

        if (T_SIZE <= buffer_size) {
            std::memcpy(buffer + end_index, &x, T_SIZE);
            end_index += T_SIZE;
        } else {
            std::memcpy(buffer + end_index, &x, buffer_size);
            end_index = BUFFER_SIZE;
            write_chunk();
            std::memcpy(buffer, &x + buffer_size, T_SIZE - buffer_size);
            end_index = T_SIZE - buffer_size;
        }
    }

    void put(std::vector<bool> const &);
};

#endif // FWRITER_H
