#include "compressor.h"

using std::pair;
using std::vector;
using std::map;
using std::multimap;

compressor::compressor(multimap<size_t, unsigned char> count_) {
    tree.reserve(count_.size());
    for (auto x: count_) {
        tree.push_back(new node<unsigned char>(x.second));
        count.insert(pair<size_t, node<unsigned char> *>(x.first, tree.back()));
    }
}

void compressor::build_tree() {
    size_t new_weight = 0;
    node<unsigned char> * ch[2];
    multimap<size_t, node<unsigned char> *>::iterator cur;
    while (count.size() > 1) {
        cur = count.begin();
        new_weight = cur->first;
        ch[0] = cur->second;
        count.erase(cur);

        cur = count.begin();
        new_weight += cur->first;
        ch[1] = cur->second;
        count.erase(cur);

        tree.push_back(new node<unsigned char>('\0', nullptr, ch[0], ch[1]));
        count.insert(pair<size_t, node<unsigned char> *>(new_weight, tree.back()));
        ch[0]->set_parent(tree.back());
        ch[1]->set_parent(tree.back());
    }
}

void compressor::encode_dfs(const node<unsigned char> * current, std::vector<bool> & code) {
    if (current->get_left_child()) {
        code.push_back(0);
        encode_dfs(current->get_left_child(), code);
    }

    if (current->get_right_child()) {
        code.push_back(1);
        encode_dfs(current->get_right_child(), code);
    }

    if (current->get_value()) {
        table.insert(pair<unsigned char, std::vector<bool>>(current->get_value(), code));
    }

    if (code.size()) {
        code.pop_back();
    }
}
