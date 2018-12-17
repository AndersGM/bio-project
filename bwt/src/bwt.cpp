#include <vector>
#include <cmath>
#include <string>
#include "util.h"
#include "cigar.h"
#include <map>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

/**
 *
 * @param o
 * @param a
 * @param i
 * @param step step size
 * @return
 */
unsigned int
O(unsigned int n, const unsigned int *o, int a, unsigned int i, const int step, const unsigned int *sa, const char *x,
  char aChar) {
    auto index = i / step;
    unsigned long entriesO = (n + (step - 1)) / step;
    unsigned long actualIndex = (a * entriesO) + index;
    unsigned int fromO = o[actualIndex];

    // calc remainder
    unsigned long j = (index * step) + 1;

    while (j <= i) {
        if (sa[j] != 0) {
            fromO += x[sa[j] - 1] == aChar ? 1 : 0;
        }
        j++;
    }

    return fromO;
}


vector<unsigned int>
BWT_search(const unsigned int n, unsigned int *sa, const char *x, const char *w, const unsigned int m,
           const unsigned int *c,
           unsigned int *o,
           const int step,
           map<int, int> &charIndexToIndex, vector<int> &indexToCharIndex) {


    auto start = std::chrono::high_resolution_clock::now();
    unsigned int l = 0;
    unsigned int r = n - 1;
    long i = m - 1;

    while (i >= 0 && l <= r) {
        int charIndex = charIndexToIndex[charToIndex(w[i])];
        l = c[charIndex] + (l < 1 ? 0 : O(n, o, charIndex, l - 1, step, sa, x, w[i])) + 1;
        r = c[charIndex] + O(n, o, charIndex, r, step, sa, x, w[i]);
        i--;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto time = end - start;

    vector<unsigned int> res;
    res.reserve(r - l + 1);

    if (i < 0 && l <= r) {
        for (unsigned int k = l; k <= r; k++) {
            res.push_back(sa[k] + 1);
        }
    }

    return res;
}

void
BWT_search_with_edit(const unsigned int n, const unsigned int *sa, const char *x, const char *w,
                     const unsigned int *c,
                     unsigned int *o,
                     const int step,
                     map<int, int> &charIndexToIndex, vector<int> &indexToCharIndex,
                     int d,
                     unsigned int w_index,
                     unsigned int l,
                     unsigned int r,
                     vector<char> &path) {
    if (w_index == -1) {
        vector<char> newPath;

        newPath.resize(path.size());    // allocate space

        reverse_copy(path.begin(), path.end(), newPath.begin());

        const string &cigar = compress_cigar(newPath);

        // give all matches between l and r
        for (unsigned long k = l; k <= r; k++) {
            // plus one for one index
            unsigned int re = sa[k];
//            res.push_back(re);
            cout << re << " " << cigar << endl;
        }

        // here we still need deletions
        if (d > 0) {
            for (int i = 0; i < indexToCharIndex.size(); i++) {
                unsigned int l_new = c[i] + (l == 0 ? 0 : O(n, o, i, l - 1, step, sa, x, char(indexToCharIndex[i]))) + 1;
                unsigned int r_new = c[i] + O(n, o, i, r, step, sa, x, char(indexToCharIndex[i]));

                if (l_new <= r_new) {
                    path.push_back('D');
                    BWT_search_with_edit(n, sa, x, w, c, o, step, charIndexToIndex, indexToCharIndex, d - 1, w_index,
                                         l_new, r_new, path);
                    path.pop_back();
                }
            }
        }

        return;
    }

    // exact match
    int cIndex = charIndexToIndex[charToIndex(w[w_index])];
    unsigned int l_new = c[cIndex] + (l == 0 ? 0 : O(n, o, cIndex, l - 1, step, sa, x, w[w_index])) + 1;
    unsigned int r_new = c[cIndex] + O(n, o, cIndex, r, step, sa, x, w[w_index]);

    path.push_back('M');
    BWT_search_with_edit(n, sa, x, w, c, o, step, charIndexToIndex, indexToCharIndex, d, w_index - 1,
                             l_new, r_new, path);

    path.pop_back();

    if (d > 0) {
        // insertion 
        path.push_back('I');
        BWT_search_with_edit(n, sa, x, w, c, o, step, charIndexToIndex, indexToCharIndex, d - 1, w_index - 1,
                             l, r, path);

        path.pop_back();

        // deletion
        for (int i = 0; i < indexToCharIndex.size(); i++) {
            l_new = c[i] + (l == 0 ? 0 : O(n, o, i, l - 1, step, sa, x, char(indexToCharIndex[i]))) + 1;
            r_new = c[i] + O(n, o, i, r, step, sa, x, char(indexToCharIndex[i]));

            if (l_new <= r_new) {
                path.push_back('D');
                BWT_search_with_edit(n, sa, x, w, c, o, step, charIndexToIndex, indexToCharIndex, d - 1, w_index,
                                     l_new, r_new, path);

                path.pop_back();
            }
        }

        // substituion
        for (int i = 0; i < indexToCharIndex.size(); i++) {
            if (w[w_index] == char(indexToCharIndex[i])){
                continue;
            }

            l_new = c[i] + (l == 0 ? 0 : O(n, o, i, l - 1, step, sa, x, char(indexToCharIndex[i]))) + 1;
            r_new = c[i] + O(n, o, i, r, step, sa, x, char(indexToCharIndex[i]));

            if (l_new <= r_new) {
                path.push_back('S');
                BWT_search_with_edit(n, sa, x, w, c, o, step, charIndexToIndex, indexToCharIndex, d - 1, w_index-1,
                                     l_new, r_new, path);

                path.pop_back();
            }
        }
    }
}
