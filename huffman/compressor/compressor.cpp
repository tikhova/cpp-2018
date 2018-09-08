#include "compressor.h"
#include "../counter/counter.h"

using std::pair;
using std::vector;
using std::map;
using std::multimap;

compressor::compressor(freader & in) : codes(SYMBOLS_AMOUNT) {
    counter cnt;
    while (!in.eof()) {
        cnt.add(in.get_chunk());
    }
    count = cnt.get();
    SYMBOLS_COUNT = count.size();
    LENGTH = cnt.get_total_amount();
}

compressor::compressor(std::multimap<size_t, size_t> const & count_)
    : LENGTH(0), SYMBOLS_COUNT(count_.size()), codes(SYMBOLS_AMOUNT), count(count_){ }

// insert counted symbols into the queue
void compressor::prepare_queue(std::vector<std::shared_ptr<node<size_t> > > & tree,
                               std::multimap<size_t, node<size_t> *> & queue) {
    if (count.size() > 1) {
        tree.reserve(SYMBOLS_COUNT * 2 - 1);
        for (auto x: count) {
            tree.push_back(std::shared_ptr<node<size_t> >(new node<size_t>(x.second)));
            queue.insert(pair<size_t, node<size_t> *>(x.first, tree.back().get()));
        }
    }
}

void compressor::build_tree(std::vector<std::shared_ptr<node<size_t> > > & tree,
                            std::multimap<size_t, node<size_t> *> & queue) {
    size_t const NEUTRAL_NODE_VALUE = SYMBOLS_AMOUNT;
    size_t new_weight = 0;
    node<size_t> * ch[2];
    multimap<size_t, node<size_t> *>::iterator cur;
    std::shared_ptr<node<size_t> > last_node;

    while (queue.size() > 1) {
        cur = queue.begin();
        new_weight = cur->first;
        ch[0] = cur->second;
        queue.erase(cur);

        cur = queue.begin();
        new_weight += cur->first;
        ch[1] = cur->second;
        queue.erase(cur);

        last_node.reset(new node<size_t>(NEUTRAL_NODE_VALUE, nullptr, ch[0], ch[1]));
        tree.push_back(last_node);
        queue.insert(pair<size_t, node<size_t> *>(new_weight, last_node.get()));
        ch[0]->set_parent(last_node.get());
        ch[1]->set_parent(last_node.get());
    }
}

// create a vector of codes (represented as bool vectors)
void compressor::encode_dfs(const node<size_t> * current, std::vector<bool> & code) {
    node<size_t> * child = current->get_left_child();
    if (child) {
        code.push_back(0);
        encode_dfs(child, code);
    }

    child = current->get_right_child();
    if (child) {
        code.push_back(1);
        encode_dfs(child, code);
    }

    if (current->get_value() < SYMBOLS_AMOUNT) {
        codes.at(current->get_value()) = code;
    }

    if (code.size()) {
        code.pop_back();
    }
}

void compressor::encode() {
    std::multimap<size_t, node<size_t> *> queue;
    std::vector<std::shared_ptr<node<size_t> > > tree;

    prepare_queue(tree, queue);
    build_tree(tree, queue);
    if (tree.size()) {
        std::vector<bool> code;
        encode_dfs(tree.back().get(), code);
    }
}

void compressor::print(freader & in, fwriter & out) {    
    encode();
    in.reset();
    // Write navigative numbers
    out.put<size_t>(LENGTH);
    out.put<size_t>(SYMBOLS_COUNT);

    if (SYMBOLS_COUNT > 1) {
        // Write the count table
        for (auto x: count) {
            out.put<size_t>(x.second);
            out.put<size_t>(x.first);
        }

        // Write encoded message
        while (!in.eof()) {
            out.put(get_code(in.get<unsigned char>()));
        }
    } else {
        while (!in.eof()) {
            out.put(in.get<unsigned char>());
        }
    }

}

std::vector<std::shared_ptr<node<size_t> > > compressor::get_tree() {
    std::multimap<size_t, node<size_t> *> queue;
    std::vector<std::shared_ptr<node<size_t> > > tree;

    prepare_queue(tree, queue);
    build_tree(tree, queue);
    return tree;
}

