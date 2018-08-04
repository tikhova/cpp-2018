#include "freader.h"
#include <fstream>

freader::freader(std::string const & filename) : reader(filename, std::ifstream::binary), fname(filename), cur_index(0), end_index(0) {
    if (reader.fail()) {
        reader.close();
        // throw exception
        throw std::runtime_error(fname + " cannot be open");
    }
}

freader::~freader() {
    reader.close();
}

void freader::read_chunk() {
    reader.read(buffer, BUF_SIZE);
    cur_index = 0;
    end_index = static_cast<size_t>(reader.gcount());
}

char freader::get() {
    if (!end_index) {
        read_chunk();
    }
    return buffer[cur_index++];
}

std::vector<char> freader::get_chunk() {
    std::vector<char> result;
    result.reserve(end_index - cur_index + 1);
    for (size_t i = cur_index; i != end_index; ++i) {
        result.push_back(buffer[i]);
    }
    return result;
}

bool freader::eof() {
    return reader.eof() && (cur_index == end_index);
}
