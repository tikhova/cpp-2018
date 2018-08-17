#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include "../freader/freader.h"
#include "../fwriter/fwriter.h"
#include "../node/node.h"
#include <vector>
#include <map>
#include <memory> // shared_ptr

class decompressor {
private:
    static constexpr size_t SYMBOLS_AMOUNT = 256;
    size_t SYMBOLS_COUNT;
    size_t LENGTH;
    std::vector<std::shared_ptr<node<size_t> > > tree;

public:
    decompressor(freader &);
    void print(freader &, fwriter &);
};

#endif // DECOMPRESSOR_H
