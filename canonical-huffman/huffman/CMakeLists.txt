cmake_minimum_required(VERSION 2.8)

project(huffman)
include_directories(${huffman_SOURCE_DIR})
add_executable(${PROJECT_NAME} main.cpp)
add_executable(huffman_test huffman_test.cpp
                            gtest/gtest-all.cc
                            gtest/gtest.h
                            gtest/gtest_main.cc)

add_library(buffered_reader_lib SHARED ../buffered_reader/buffered_reader.h ../buffered_reader/buffered_reader.cpp)
add_library(buffered_writer_lib SHARED ../buffered_writer/buffered_writer.h ../buffered_writer/buffered_writer.cpp)
add_library(huffman_lib SHARED huffman.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(${PROJECT_NAME} huffman_lib buffered_reader_lib buffered_writer_lib)
target_link_libraries(huffman_test -lgmp -lgmpxx -lpthread huffman_lib buffered_reader_lib buffered_writer_lib)
