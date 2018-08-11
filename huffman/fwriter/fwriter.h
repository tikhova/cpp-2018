#ifndef FWRITER_H
#define FWRITER_H

#include <string>
#include <vector>
#include <fstream>

class  fwriter {
private:
    static constexpr size_t BUF_SIZE = 1024;
    static constexpr size_t CHAR_SIZE = 8;
    std::ofstream writer;
    unsigned char current;
    size_t bit_index;

public:
    fwriter(std::string const &filename);
    ~fwriter();

    template <typename T>
    inline void put(T x) { writer << x; }

    void put(std::vector<bool>);
};

#endif // FWRITER_H
