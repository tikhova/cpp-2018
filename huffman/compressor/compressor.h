#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../node/node.h"
#include <vector>
#include <map>

class compressor {
private:
    std::multimap<size_t, node<unsigned char> *> count;
    std::map<unsigned char, std::vector<bool>> table;
    std::vector<node<unsigned char> *> tree;


    void build_tree();    
    void encode_dfs(const node<unsigned char> *, std::vector<bool> &);

public:
    compressor(std::multimap<size_t, unsigned char>);
    inline void encode() { build_tree(); std::vector<bool> code; encode_dfs(tree.back(), code); }
    inline std::map<unsigned char, std::vector<bool>> & get_table() { return table;}
    inline std::vector<bool> & get_code(unsigned char c) { return table.find(c)->second; }
};

void set_bit(size_t & code, int bit, size_t position);

#endif // COMPRESSOR_H
