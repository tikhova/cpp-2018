#include <iostream>
#include <ctime>
#include <iomanip>
#include "huffman.cpp"

using namespace std;

int main(int argc, char **argv) {
    enum {
        COMPRESS, DECOMPRESS
    } type;
    const string USAGE_MESSAGE = "Expected usage: -c/-d source_name [destination_name]\n";
    if (argc < 3 || argc > 4) {
        std::cerr << USAGE_MESSAGE;
        return -1;
    }
    if (std::strcmp(argv[1], "-c") == 0) {
        type = COMPRESS;
    } else if (std::strcmp(argv[1], "-d") == 0) {
        type = DECOMPRESS;
    } else {
        std::cerr << USAGE_MESSAGE;
        return -1;
    }

    std::string source_name = argv[2];
    std::string destination_name = argc > 3 ? argv[3] : source_name + "_hff";

    ifstream in(source_name, std::ifstream::binary);
    ofstream out(destination_name, std::ifstream::binary);

    std::clock_t time = clock();
    switch (type) {
        case COMPRESS:
            {
                std::cout << "Compressing..." << std::endl;
                encode(in, out);
            }
            break;
        case DECOMPRESS:
            {
                std::cout << "Decompressing..." << std::endl;
                decode(in, out);
            }
            break;
    }
    std::cout << (type == COMPRESS ? "Compression" : "Decompression") << " took: "
                  << std::setprecision(3) << double(clock() - time) / CLOCKS_PER_SEC << "s."
                  << std::endl;
}
