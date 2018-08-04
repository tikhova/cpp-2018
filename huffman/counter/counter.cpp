#include "counter.h"

#include <cstring>
#include <iterator>

using std::pair;
using std::vector;
using std::multimap;

counter::counter() {
    count.reserve(SYMBOLS_AMOUNT);
    for (size_t i = 0; i != SYMBOLS_AMOUNT; ++i) {
        count.push_back(0);
    }
}

void counter::add(char const c) {
    ++count.at(static_cast<unsigned char>(c));
}

void counter::add(vector<char> const & a) {
    for (auto c: a) {
        add(c);
    }
}

size_t counter::get_count(char const c) {
    return count.at(static_cast<unsigned char>(c));
}

multimap<size_t, unsigned char> counter::get() {
    multimap<size_t, unsigned char> result;
    for (size_t i = 0; i != SYMBOLS_AMOUNT; ++i) {
        if (count.at(i)) {
            result.insert(pair<size_t, unsigned char>(count.at(i), static_cast<unsigned char>(i)));
        }
    }
    return result;
}
