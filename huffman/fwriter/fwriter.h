#ifndef FWRITER_H
#define FWRITER_H

#include <string>
#include <vector>
#include <fstream>

class  fwriter {
private:
    static constexpr size_t BUF_SIZE = 4096;
    std::ofstream writer;
    std::string fname; // will I need it?
    char buffer[BUF_SIZE];
    size_t cur_index;
    size_t bit_index;

public:
    fwriter(std::string const &filename);
    ~fwriter();

    void put(char const);
    void put(std::vector<bool> const);
};

#endif // FWRITER_H
