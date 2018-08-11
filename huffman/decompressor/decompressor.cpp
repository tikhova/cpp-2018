#include "decompressor.h"

decompressor::decompressor(freader & in) {
    LENGTH = in.get<size_t>();
    SYMBOLS_COUNT = in.get<size_t>();
    BITS_COUNT = in.get<size_t>();
    size_t last_length = in.get<size_t>();

    std::vector<unsigned char> characters;
    characters.reserve(SYMBOLS_COUNT);
    std::vector<bool> previous;
    std::vector<bool> current;
    size_t cur_length = 0;

    in.skipws(false);
    for (size_t i = 0; i != SYMBOLS_COUNT; ++i) {
        characters.push_back(in.get<unsigned char>());
    }

    previous = in.get_bool_vector(BITS_COUNT);
    for (size_t i = 1; i != SYMBOLS_COUNT; ++i) {
        current = in.get_bool_vector(BITS_COUNT);
        cur_length = BITS_COUNT - matching_length(previous, current);
        swap(previous, current);
        current.resize(cur_length);
        table.insert(std::pair<std::vector<bool>, unsigned char>(current, characters.at(i - 1)));
    }

    previous.resize(last_length);
    table.insert(std::pair<std::vector<bool>, unsigned char>(previous, characters.back()));
}

size_t matching_length(std::vector<bool> first, std::vector<bool> second) {
    size_t result = 0;
    size_t first_length = first.size();
    size_t second_length = second.size();
    for (; first_length-- != 0 && second_length-- != 0 &&
         first.at(first_length) == second.at(second_length); ++result);
    return result;
}

void decompressor::print(freader & in, fwriter & out) {
    std::vector<bool> current;
    while (!in.eof() && LENGTH) {
        current.push_back(in.get_bit());
        if (table.find(current) != table.end()) {
            out.put(table.find(current)->second);
            current.clear();
            --LENGTH;
        }
    }
}
