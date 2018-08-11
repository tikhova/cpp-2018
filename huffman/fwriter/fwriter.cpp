#include "fwriter.h"

fwriter::fwriter(std::string const &filename) :
    writer(filename, std::ifstream::binary), current(0), bit_index(0) {
    if (writer.fail()) {
        writer.close();
        throw std::runtime_error(filename + " cannot be written to");
    } else {
        writer << std::noskipws;
    }
}

fwriter::~fwriter() {    
    if (bit_index % CHAR_SIZE != 0) {
        put(current);
    }

    writer.close();
}

void fwriter::put(std::vector<bool> a) {
    for (auto x: a) {
        if (bit_index == CHAR_SIZE) {
            put(current);
            bit_index = 0;
            current = 0;
        }
        current |= (x << (CHAR_SIZE - 1 - bit_index++));
    }
}
