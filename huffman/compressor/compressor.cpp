#include "compressor.h"

#include "../counter/counter.h"

using std::pair;
using std::vector;
using std::map;
using std::multimap;

compressor::compressor(freader & in) {
    counter cnt;
    while (!in.eof()) {
        cnt.add(in.get_chunk());
    }
    *this = compressor(cnt.get());
    LENGTH = cnt.get_total_amount();
    in.reset();
}

compressor::compressor(multimap<size_t, size_t> count_) {
    tree.reserve(count_.size());
    for (auto x: count_) {
        tree.push_back(std::shared_ptr<node<size_t> >(new node<size_t>(x.second)));
        count.insert(pair<size_t, node<size_t> *>(x.first, tree.back().get()));
    }
    encode();
}

void compressor::build_tree() {
    size_t const NEUTRAL_NODE_VALUE = SYMBOLS_AMOUNT;
    size_t new_weight = 0;
    node<size_t> * ch[2];
    multimap<size_t, node<size_t> *>::iterator cur;
    while (count.size() > 1) {
        cur = count.begin();
        new_weight = cur->first;
        ch[0] = cur->second;
        count.erase(cur);

        cur = count.begin();
        new_weight += cur->first;
        ch[1] = cur->second;
        count.erase(cur);

        tree.push_back(std::shared_ptr<node<size_t> >(new node<size_t>(NEUTRAL_NODE_VALUE, nullptr, ch[0], ch[1])));
        count.insert(pair<size_t, node<size_t> *>(new_weight, tree.back().get()));
        ch[0]->set_parent(tree.back().get());
        ch[1]->set_parent(tree.back().get());
    }
}

void compressor::encode_dfs(const node<size_t> * current, std::vector<bool> & code) {
    if (current->get_left_child()) {
        code.push_back(0);
        encode_dfs(current->get_left_child(), code);
    }

    if (current->get_right_child()) {
        code.push_back(1);
        encode_dfs(current->get_right_child(), code);
    }

    if (current->get_value() < SYMBOLS_AMOUNT) {
        table.insert(pair<unsigned char, std::vector<bool>>(static_cast<unsigned char>(current->get_value()), code));
    }

    if (code.size()) {
        MAX_CODE_LENGTH = std::max(code.size(), MAX_CODE_LENGTH);
        code.pop_back();
    }
}

void compressor::print(freader & in, fwriter & out) {
    out.put<size_t>(LENGTH);
    out.put<char>('\n');
    out.put<size_t>(table.size());
    out.put<char>('\n');
    out.put<size_t>(MAX_CODE_LENGTH);
    out.put<char>('\n');

    map<std::vector<bool>, unsigned char> reversed_table;

    for(map<unsigned char, std::vector<bool>>::iterator it = table.begin(); it != table.end(); ++it) {
        reversed_table.insert(pair<std::vector<bool>, unsigned char>(resized_bool_vector(it->second, MAX_CODE_LENGTH),
                                                                     it->first));
    }

    out.put<size_t>(table.find(reversed_table.rbegin()->second)->second.size());

    for(map<std::vector<bool>, unsigned char>::iterator it = reversed_table.begin(); it != reversed_table.end(); ++it) {
        out.put(it->second);
    }

    for(map<std::vector<bool>, unsigned char>::iterator it = reversed_table.begin(); it != reversed_table.end(); ++it) {
        out.put(it->first);
    }

    while (!in.eof()) {
        out.put(get_code(in.get_char()));
    }
}

std::vector<bool> resized_bool_vector(std::vector<bool> const & x, size_t length) {
    std::vector<bool> result = x;
    result.resize(length);
    return result;
}

std::vector<bool> & compressor::get_code(unsigned char c) {
    if (table.find(c) != table.end()) {
        return table.find(c)->second;
    }
    throw std::runtime_error("don't have a code for" + std::string(1, static_cast<char>(c)));
}
