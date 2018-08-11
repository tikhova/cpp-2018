#include "freader.h"
#include <fstream>

freader::freader(std::string const & filename) :
    reader(filename, std::ifstream::binary), bit_index(0), cur_index(0), end_index(0) {
    if (reader.fail()) {
        reader.close();
        throw std::runtime_error(filename + " cannot be open");
    }
}

freader::~freader() {
    reader.close();
}

void freader::read_chunk() {
    reader.read((char *)buffer, BUF_SIZE);
    cur_index = 0;
    bit_index = 0;
    end_index = static_cast<size_t>(reader.gcount());
}

unsigned char freader::get_char() {
    if (cur_index == end_index) {
        read_chunk();
    }
    return static_cast<unsigned char>(buffer[cur_index++]);
}

//unsigned char freader::get_char() {
//    unsigned char res;
//    reader >> res;
//    return res;
//}

bool freader::get_bit() {
    if (bit_index == CHAR_SIZE) {
        bit_index = 0;
        cur_index++;
    }


    if (cur_index == end_index) {
        read_chunk();
    }

    return buffer[cur_index] & (1 << (CHAR_SIZE - 1 - bit_index++));
}

std::vector<bool> freader::get_bool_vector(size_t n) {
    std::vector<bool> result;
    result.reserve(n);
    while (n--) {
        result.push_back(get_bit());
    }
    return result;
}

std::vector<unsigned char> freader::get_chunk() {
    if (cur_index == end_index) {
        read_chunk();
    }
    std::vector<unsigned char> result;
    result.reserve(end_index - cur_index + 1);
    for (size_t i = cur_index; i != end_index; ++i) {
        result.push_back(buffer[i]);
    }
    cur_index = end_index = 0;
    return result;
}

bool freader::eof() {
    return reader.eof() && (cur_index == end_index) /*&& (bit_index % CHAR_SIZE == 0)*/;
}
