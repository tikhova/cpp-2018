#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../node/node.h"
#include "../freader/freader.h"
#include "../fwriter/fwriter.h"

#include <vector>
#include <map>
#include <string>
#include <memory> // shared_ptr

class compressor {
private:
    static constexpr size_t SYMBOLS_AMOUNT = 256;
    size_t LENGTH; // length of the input
    size_t SYMBOLS_COUNT; // count of symbols in the input
    std::vector<std::vector<bool>> codes;
    std::multimap<size_t, size_t> count;

    void prepare_queue(std::vector<std::shared_ptr<node<size_t> > > &, std::multimap<size_t, node<size_t> *> &);
    void build_tree(std::vector<std::shared_ptr<node<size_t> > > &, std::multimap<size_t, node<size_t> *> &);
    void encode_dfs(node<size_t> const *, std::vector<bool> &);
    void encode();
    inline std::vector<bool> & get_code(unsigned char const & c) { return codes.at(c); }

public:
    compressor(freader &);
    compressor(std::multimap<size_t, size_t> const &);

    void print(freader &, fwriter &);
    std::vector<std::shared_ptr<node<size_t> > > get_tree();
};

#endif // COMPRESSOR_H
