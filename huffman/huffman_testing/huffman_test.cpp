#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <gtest/gtest.h>
#include "../freader/freader.h"
#include "../fwriter/fwriter.h"
#include "../compressor/compressor.h"
#include "../decompressor/decompressor.h"

void compress_file(std::string source_name, std::string destination_name) {
    freader source(source_name);
    fwriter destination(destination_name);
    compressor comp(source);
    comp.print(source, destination);
}

void decompress_file(std::string source_name, std::string destination_name) {
    freader source(source_name);
    fwriter destination(destination_name);
    decompressor decomp(source);
    decomp.print(source, destination);
}

bool equal_files(std::string first_name, std::string second_name) {
    freader first(first_name);
    freader second(second_name);
    
    std::vector<unsigned char> first_chunk;
    std::vector<unsigned char> second_chunk;
    
    while (!first.eof() && !second.eof()) {
        first_chunk = first.get_chunk();
        second_chunk = second.get_chunk();
        if (first_chunk != second_chunk) return false;
    }
    
    if (first.eof() != second.eof()) return false;
    return true;
}

TEST(correctness, empty)
{
    compress_file("precompressed/empty.txt", "compressed/empty.hff");
    decompress_file("compressed/empty.hff", "decompressed/empty_decomp.txt");
    EXPECT_TRUE(equal_files("precompressed/empty.txt", "decompressed/empty_decomp.txt"));
}

TEST(correctness, one_symbol)
{
    compress_file("precompressed/one_symbol.txt", "compressed/one_symbol.hff");
    decompress_file("compressed/one_symbol.hff", "decompressed/one_symbol_decomp.txt");
    EXPECT_TRUE(equal_files("precompressed/one_symbol.txt", "decompressed/one_symbol_decomp.txt"));
}


TEST(correctness, picture)
{
    compress_file("precompressed/picture.jpg", "compressed/picture.hff");
    decompress_file("compressed/picture.hff", "decompressed/picture_decomp.jpg");
    EXPECT_TRUE(equal_files("precompressed/picture.jpg", "decompressed/picture_decomp.jpg"));
}

TEST(correctness, fb2_file)
{
    compress_file("precompressed/ebook.fb2", "compressed/ebook.hff");
    decompress_file("compressed/ebook.hff", "decompressed/ebook_decomp.fb2");
    EXPECT_TRUE(equal_files("precompressed/ebook.fb2", "decompressed/ebook_decomp.fb2"));
}

TEST(correctness, pdf_file)
{
    compress_file("precompressed/pdf_file.pdf", "compressed/pdf_file.hff");
    decompress_file("compressed/pdf_file.hff", "decompressed/pdf_file_decomp.pdf");
    EXPECT_TRUE(equal_files("precompressed/pdf_file.pdf", "decompressed/pdf_file_decomp.pdf"));
}

TEST(correctness, audio)
{
    compress_file("precompressed/audio.mp3", "compressed/audio.hff");
    decompress_file("compressed/audio.hff", "decompressed/audio_decomp.mp3");
    EXPECT_TRUE(equal_files("precompressed/audio.mp3", "decompressed/audio_decomp.mp3"));
}

TEST(correctness, video)
{
    compress_file("precompressed/video.mp4", "compressed/video.hff");
    decompress_file("compressed/video.hff", "decompressed/video_decomp.mp4");
    EXPECT_TRUE(equal_files("precompressed/video.mp4", "decompressed/video_decomp.mp4"));
}

TEST(correctness, big_file)
{
    compress_file("precompressed/big_file.tzt", "compressed/big_file.hff");
    decompress_file("compressed/big_file.hff", "decompressed/big_file_decomp.tzt");
    EXPECT_TRUE(equal_files("precompressed/big_file.tzt", "decompressed/big_file_decomp.tzt"));
}

TEST(correctness, long_codes)
{
    compress_file("precompressed/long_codes.txt", "compressed/long_codes.hff");
    decompress_file("compressed/long_codes.hff", "decompressed/long_codes_decomp.txt");
    EXPECT_TRUE(equal_files("precompressed/long_codes.txt", "decompressed/long_codes_decomp.txt"));
}

TEST(correctness, human_text)
{
    compress_file("precompressed/human_text.txt", "compressed/human_text.hff");
    decompress_file("compressed/human_text.hff", "decompressed/human_text_decomp.txt");
    EXPECT_TRUE(equal_files("precompressed/human_text.txt", "decompressed/human_text_decomp.txt"));
}

TEST(correctness, compress_only)
{
    compress_file("precompressed/big_file.tzt", "compressed/big_file.hff");
}

TEST(correctness, decompress_only)
{
    decompress_file("compressed/big_file.hff", "decompressed/big_file_decomp.tzt");
}




