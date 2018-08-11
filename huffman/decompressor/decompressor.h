#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "../freader/freader.h"
#include "../fwriter/fwriter.h"
#include <vector>
#include <map>

class decompressor {
private:
    size_t SYMBOLS_COUNT;
    size_t BITS_COUNT;
    size_t LENGTH;
    std::map<std::vector<bool>, unsigned char> table;

public:
    decompressor(freader &);
    void print(freader &, fwriter &);
};

size_t matching_length(std::vector<bool>, std::vector<bool>);

#endif // DECOMPRESSOR_H
