#include "fwriter.h"

fwriter::fwriter(std::string const &filename) :
    writer(filename, std::ifstream::binary), current(0), bit_index(0), end_index(0) {
    if (writer.fail()) {
        writer.close();
        throw std::runtime_error(filename + " cannot be written to");
    }
}

fwriter::~fwriter() {
    if (bit_index) {
        put(current);
    }

    write_chunk();
    writer.close();
}

void fwriter::put(std::vector<bool> const & a) {
    for (auto x: a) {
        if (bit_index == CHAR_SIZE) {
            put(current);
            bit_index = current = 0;
        }
        current |= (static_cast<size_t>(x) << (CHAR_SIZE - ++bit_index));
    }
}

void fwriter::write_chunk() {
        writer.write((char *)buffer, static_cast<std::streamsize>(end_index));
        bit_index = 0;
        end_index = 0;
}
