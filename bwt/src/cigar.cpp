#include "cigar.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

string compress_cigar(vector<char> path) {
    if (path.empty()) {
        return "";
    }

    stringstream ss;
    unsigned long i = 0, matches;
    unsigned long n = path.size();

    while (i < n) {
        matches = 1;
        while (i < n - 1 && (path[i] == path[i + 1])) {
            matches++;
            i++;
        }
        ss << matches << path[i];
        i++;
    }
    return ss.str();
}
