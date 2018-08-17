#include "counter.h"

#include <cstring>
#include <iterator>

using std::pair;
using std::vector;
using std::multimap;

counter::counter() : count(SYMBOLS_AMOUNT, 0) {}

void counter::add(unsigned char const & c) {
    ++count.at(c);
    ++LENGTH;
}

void counter::add(vector<unsigned char> const & a) {
    for (auto c: a) {
        add(c);
    }
}

multimap<size_t, size_t> counter::get() {
    multimap<size_t, size_t> result;
    for (size_t i = 0; i != SYMBOLS_AMOUNT; ++i) {
        if (count.at(i)) {
            result.insert(pair<size_t, size_t>(count.at(i), i));
        }
    }
    return result;
}
