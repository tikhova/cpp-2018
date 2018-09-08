cmake_minimum_required(VERSION 2.8)

project(UTIL)

include_directories(${UTIL_SOURCE_DIR})

add_executable(main
               main.cpp
               ../freader/freader.h
               ../freader/freader.cpp
               ../fwriter/fwriter.h
               ../fwriter/fwriter.cpp
               ../counter/counter.h
               ../counter/counter.cpp
               ../compressor/compressor.h
               ../compressor/compressor.cpp
               ../decompressor/decompressor.h
               ../decompressor/decompressor.cpp)

           if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
             set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
             set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
           endif()

           target_link_libraries(main -lgmp -lgmpxx -lpthread)
