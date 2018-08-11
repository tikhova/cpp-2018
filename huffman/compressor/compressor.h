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
    size_t MAX_CODE_LENGTH = 0;
    size_t LENGTH = 0;
    std::multimap<size_t, node<size_t> *> count;
    std::map<unsigned char, std::vector<bool>> table;
    std::vector<std::shared_ptr<node<size_t> > > tree;


    void build_tree();    
    void encode_dfs(node<size_t> const *, std::vector<bool> &);
    inline void encode() { build_tree(); std::vector<bool> code; encode_dfs(tree.back().get(), code); }
    std::vector<bool> & get_code(unsigned char);
    compressor(std::multimap<size_t, size_t>);

public:
    compressor(freader &);
    void print(freader &, fwriter &);
};

std::vector<bool> resized_bool_vector(std::vector<bool> const &, size_t);
void reverse(std::vector<bool> &);

#endif // COMPRESSOR_H
