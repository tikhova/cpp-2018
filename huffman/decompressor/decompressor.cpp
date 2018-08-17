#include "decompressor.h"
#include "../compressor/compressor.h"

decompressor::decompressor(freader & in) {
    // Read navigative numbers
    LENGTH = in.get<size_t>();
    SYMBOLS_COUNT = in.get<size_t>();    

    if (SYMBOLS_COUNT > 1) {
        std::multimap<size_t, size_t> count;

        // Read the count table
        for (size_t i = 0; i != SYMBOLS_COUNT; ++i) {
            count.insert(std::pair<size_t, size_t>(in.get<size_t>(), in.get<size_t>()));
        }

        tree = compressor(count).get_tree();
    }
}

void decompressor::print(freader & in, fwriter & out) {
    if (SYMBOLS_COUNT > 1) {
        node<size_t> * current = tree.back().get();
        bool bit;
        while (LENGTH) {
            bit = in.get_bit();
            current = bit ? current->get_right_child() : current->get_left_child();
            if (current->get_value() < SYMBOLS_AMOUNT) {
                out.put<unsigned char>(static_cast<unsigned char>(current->get_value()));
                current = tree.back().get();
                --LENGTH;
            }
        }
    } else {
        while (!in.eof()) {
            out.put(in.get<unsigned char>());
        }
    }
}
