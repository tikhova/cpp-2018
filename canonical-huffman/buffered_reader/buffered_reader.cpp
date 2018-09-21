#include "buffered_reader.h"
#include <fstream>

buffered_reader::buffered_reader(std::istream &stream) :
    reader(stream), cur_index(0), end_index(0) {
    if (reader.fail()) {
        throw std::runtime_error("Buffered reader error: cannot be open");
    } else {
        read_chunk();
    }
}

void buffered_reader::read_chunk() {
    reader.read(buffer, BUFFER_SIZE);
    cur_index = 0;
    end_index = static_cast<size_t>(reader.gcount());
}

void buffered_reader::reset() {
    reader.clear();
    reader.seekg(0);
    cur_index = end_index = 0;
}
