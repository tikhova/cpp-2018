#include <iostream>
#include "../freader/freader.h"
#include "../fwriter/fwriter.h"
#include "../counter/counter.h"
#include "../compressor/compressor.h"
#include "../decompressor/decompressor.h"
#include <fstream>
#include <algorithm>
#include <climits>
#include <ctime>
#include <iomanip>

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    enum {
        COMPRESS, DECOMPRESS
    } type;
    type = std::strcmp(argv[1], "-c") == 0 ? COMPRESS : DECOMPRESS;

    std::string source_name = argv[2];
    std::string destination_name = argc > 3 ? argv[3] : source_name + "_hff";

    freader source(source_name);
    fwriter destination(destination_name);
    
    std::clock_t time = clock();
    std::cout << "please, await" << std::endl;
    switch (type) {
        case COMPRESS:
            {
                compressor comp(source);
                comp.print(source, destination);
            }
            break;
        case DECOMPRESS:
            {
                decompressor decomp(source);
                decomp.print(source, destination);
            }
    }
    std::cerr << (type == COMPRESS ? "compression" : "decompression") << " took: "
                  << std::setprecision(3) << double(clock() - time) / CLOCKS_PER_SEC << "s."
                  << std::endl;
}
