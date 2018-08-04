#include "fwriter.h"

fwriter::fwriter(std::string const &filename) :
    writer(filename, std::ifstream::binary), fname(filename), cur_index(0), bit_index(0) {
    if (writer.fail()) {
        writer.close();
        throw std::runtime_error(fname + " cannot be written to");
    }
}

fwriter::~fwriter() {
    writer.write(buffer, static_cast<std::streamsize>(cur_index));
    writer.close();
}

void fwriter::put(char const c) {
    if (cur_index == BUF_SIZE) {
        writer.write(buffer, static_cast<std::streamsize>(cur_index));
        cur_index = 0;
    }
    buffer[cur_index++] = c;
}

void fwriter::put(std::vector<bool> const a) {
    char c = 0;
    for (auto x: a) {
        if (bit_index == 8) {
            put(c);
            bit_index = 0;
            c = 0;
        }
        c |= (x << bit_index++);
    }
}
