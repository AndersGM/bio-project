cmake_minimum_required(VERSION 2.8.9)
project (bwt)
set(src src/cigar.h src/cigar.cpp src/util.cpp src/util.h)

add_executable(bwt src/main.cpp ${src})
add_executable(bwt_test test/test.cpp ${src})
set(CMAKE_BUILD_TYPE Release)