#include <assert.h>
#include "../src/cigar.h"
#include <vector>
#include <iostream>

void test_compressCigar();

using namespace std;

int main() {
    test_compressCigar();
    return 0;
}

void test_compressCigar() {
    auto path = vector<char>();
    assert (compress_cigar(path).empty());
    path.push_back('I');
    path.push_back('M');
    assert (compress_cigar(path) == "1I1M");
    path.push_back('M');
    path.push_back('M');
    path.push_back('M');
    path.push_back('M');
    path.push_back('M');
    path.push_back('M');
    assert (compress_cigar(path) == "1I7M");
    path.push_back('S');
    path.push_back('S');
    path.push_back('S');
    path.push_back('S');
    assert (compress_cigar(path) == "1I7M4S");
    path.push_back('D');
    assert (compress_cigar(path) == "1I7M4S1D");
}