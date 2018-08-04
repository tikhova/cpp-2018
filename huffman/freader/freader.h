#pragma once
#ifndef FREADER_H
#define FREADER_H

#include <string>
#include <fstream>
#include <vector>

class freader {
private:
    static constexpr size_t BUF_SIZE = 4096;
    std::ifstream reader;
    std::string fname; // will I need it?
    char buffer[BUF_SIZE];
    size_t cur_index;
    size_t end_index;

public:
    freader(std::string const &);
    ~freader();

    void read_chunk();
    char get();
    std::vector<char> get_chunk();
    bool eof();
};

#endif // FREADER_H
