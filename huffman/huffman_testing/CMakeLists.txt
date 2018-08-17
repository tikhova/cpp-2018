cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_TESTING)

include_directories(${HUFFMAN_TESTING_SOURCE_DIR})

add_executable(huffman_test
               huffman_test.cpp
               ../freader/freader.h
               ../freader/freader.cpp
               ../fwriter/fwriter.h
               ../fwriter/fwriter.cpp
               ../counter/counter.h
               ../counter/counter.cpp
               ../compressor/compressor.h
               ../compressor/compressor.cpp
               ../decompressor/decompressor.h
               ../decompressor/decompressor.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)

           if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
             set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
             set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
           endif()

           target_link_libraries(huffman_test -lgmp -lgmpxx -lpthread)
