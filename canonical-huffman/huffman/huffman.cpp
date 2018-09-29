#ifndef HUFFMAN
#define HUFFMAN

#include "../buffered_reader/buffered_reader.h"
#include "../buffered_writer/buffered_writer.h"
#include <queue>
#include <map>
#include <cmath> // pow

const int MAX_LENGTH = sizeof (unsigned int) * 8;

struct node {
    size_t value;
    int left;
    int right;
};

void count_amount(buffered_reader &in, unsigned int counts[256]) {
    unsigned char c;
    while(in.get<unsigned char>(c)) {
        counts[c]++;
    }
    in.reset();
}

void build_tree(unsigned int counts[256], std::vector<node> &tree) {
    std::priority_queue<std::pair<size_t, int>, std::vector<std::pair<size_t, int> >, std::greater<std::pair<size_t, int> > > queue;
    int n = 0;
    for (int i = 0; i < 256; ++i) {
        if(counts[i]) {
            queue.push({counts[i], n++});
            tree.push_back({static_cast<size_t>(i), -1, -1});
        }
    }

    size_t const NEUTRAL_NODE_VALUE = 256;
    tree.reserve(static_cast<size_t>(std::max(0, n * 2 - 1)));
    std::pair<size_t, int> first, second;
    while (queue.size() > 1) {
        first = queue.top();
        queue.pop();

        second = queue.top();
        queue.pop();

        tree.push_back({NEUTRAL_NODE_VALUE, first.second, second.second});
        queue.push({first.first + second.first, n++});
    }
    if (queue.size() > 0 && n == 1) {
        tree.push_back({NEUTRAL_NODE_VALUE, -1, second.second});
    }
}

void count_code_length(int current, std::vector<node> &tree, int length, int length_array[256]) {
    node cur = tree.at(static_cast<size_t>(current));
    int child = cur.left;
    if (child != -1) {
        count_code_length(child, tree, length + 1, length_array);
    }

    child = cur.right;
    if (child != -1) {
        count_code_length(child, tree, length + 1, length_array);
    }

    if (cur.value < 256) {
        length_array[cur.value] = length;
    }
}

void prepare_start_values(int length[256], unsigned int start_value[MAX_LENGTH + 1], unsigned int length_count[MAX_LENGTH + 1]) {
    size_t m = 0;
    for (size_t i = 0; i < 256; ++i) {
        m = length[i] > m ? length[i] : m;
        ++length_count[length[i]];
    }

    for (size_t i = m; i + 1 != 0; --i) {
        start_value[i] = (start_value[i + 1] + length_count[i + 1]) >> 1;
    }
}

void get_code(int length[256], unsigned int code[256]) {
    unsigned int start_value[MAX_LENGTH + 1] = {};
    unsigned int length_count[MAX_LENGTH + 1] = {};

    prepare_start_values(length, start_value, length_count);

    for (size_t i = 0; i < 256; ++i) {
        if (length[i]) {
            code[i] = start_value[length[i]]++;
        }
    }
}

void encode(std::istream &in_stream, std::ostream &out_stream) {
    buffered_reader in(in_stream);
    buffered_writer out(out_stream);

    unsigned int counts[256] = {};
    count_amount(in, counts);

    std::vector<node> tree;
    build_tree(counts, tree);

    int length[256] = {};
    if (tree.size() > 1) {
        count_code_length(static_cast<int>(tree.size() - 1), tree, 0, length);
    }

    unsigned int code[256] = {};
    get_code(length, code);

    unsigned long long message_length = 0;
    for (int i = 0; i != 256; ++i) {
        message_length += length[i] * counts[i];
    }


    // print the result
    out.put(message_length);

    for (size_t i = 0; i != 256; ++i) {
        out.put(length[i]);
    }

    int bits_count = 0;
    unsigned int current = 0;
    unsigned char c;
    while(in.get<unsigned char>(c)) {
        for (int i = length[c] - 1; i >= 0; --i) {
            if (bits_count == MAX_LENGTH) {
                out.put(current);
                bits_count = current = 0;
            }
            current |= ((code[c] >> i) & 1) << (MAX_LENGTH - 1 - bits_count++);
        }
    }
    if (bits_count) {
        out.put(current);
    }

}

struct decoding_table_line {
    unsigned int start_value;
    int code_length;
    std::vector<unsigned char> characters = {};

    decoding_table_line(unsigned int sv, int cl) : start_value(sv), code_length(cl) {}
};

void build_decoding_table(int length[256], std::vector<decoding_table_line> &result) {
    unsigned int start_value[MAX_LENGTH + 1] = {};
    unsigned int length_count[MAX_LENGTH + 1] = {};

    prepare_start_values(length, start_value, length_count);

    for (size_t i = MAX_LENGTH; i > 0 && i <= MAX_LENGTH; --i) {
        start_value[i] <<= (MAX_LENGTH - i);
    }

    size_t n = 0;
    for (size_t i = 0; i != MAX_LENGTH + 1; ++i) {
        if (length_count[i]) {
            ++n;
        }
    }

    result.reserve(n);
    for (int i = MAX_LENGTH; i != 0; --i) {
        if (length_count[i]) {
            result.push_back(decoding_table_line(start_value[i], i));
            for (size_t j = 0; j < 256; ++j) {
                if (length[j] == i) {
                    result.back().characters.push_back(static_cast<unsigned char>(j));
                }
            }
        }
    }
}

void decode(std::istream &in_stream, std::ostream &out_stream) {
    buffered_reader in(in_stream);
    buffered_writer out(out_stream);

    // reading information
    unsigned long long message_length;
    if (!in.get<unsigned long long>(message_length)) {
        throw std::runtime_error("File is corrupted: did not recieve message length");
    }    

    int length[256];
    for (size_t i = 0; i < 256; ++i) {
        in.get<int>(length[i]);
    }

    // build table
    std::vector<decoding_table_line> table;
    build_decoding_table(length, table);

    // decode message
    int bits_used = MAX_LENGTH;
    int bits_needed = MAX_LENGTH;
    unsigned int last_read = 0;
    unsigned int current = 0;
    while(message_length) {
        while (bits_needed) {
            if (bits_used == MAX_LENGTH) {
                if (in.get<unsigned int>(last_read)) {
                    bits_used = 0;
                } else {
                    bits_needed = 0;
                }
            } else {
                current |= (last_read << bits_used) >> (MAX_LENGTH - bits_needed);
                if (MAX_LENGTH - bits_used > bits_needed) {
                    bits_used += bits_needed;
                    bits_needed = 0;
                } else {
                    bits_needed -= (MAX_LENGTH - bits_used);
                    bits_used = MAX_LENGTH;
                }
            }
        }

        size_t table_index;
        for (table_index = table.size() - 1; table_index < table.size(); --table_index) {
            if (table.at(table_index).start_value <= current) {
                break;
            }
        }


        int lookup_index = (current - table.at(table_index).start_value) >> (MAX_LENGTH - table.at(table_index).code_length);
        out.put(table.at(table_index).characters.at(lookup_index));
        bits_needed = table.at(table_index).code_length;
        current <<= bits_needed;
        message_length -= bits_needed;
   }

}

#endif
