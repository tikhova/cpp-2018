#ifndef buffered_writer_H
#define buffered_writer_H

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <climits>

class  buffered_writer {
private:
    static constexpr size_t BUFFER_SIZE = 1 << 18;
    static constexpr size_t CHAR_SIZE = CHAR_BIT;
    char buffer[BUFFER_SIZE];
    std::ostream& writer;
    size_t end_index;

    void write_chunk();

public:
    buffered_writer(std::ostream &);
    ~buffered_writer();

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
};

#endif // buffered_writer_H
