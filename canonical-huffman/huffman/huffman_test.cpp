#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <gtest/gtest.h>
#include <iostream>
#include "huffman.cpp"

std::string run_through_huffman(std::string &s) {
    std::istringstream in(s);
    std::ostringstream out_h;
    encode(in, out_h);

    std::istringstream in_h(out_h.str());
    std::ostringstream out;
    decode(in_h, out);

    return out.str();

}

std::string random_string(std::size_t size) {
    std::string s(size, 0);
    std::generate_n(s.begin(), size, []{ return char(std::rand()); });
    return s;
}


TEST(correctness, empty_string) {
    std::string s;
    EXPECT_EQ(s, run_through_huffman(s));
}

TEST(correctness, one_symbol) {
    std::string s(100, 'a');
    EXPECT_EQ(s, run_through_huffman(s));
}

TEST(correctness, big_string) {
    std::string s = random_string(1000000);
    EXPECT_EQ(s, run_through_huffman(s));
}





