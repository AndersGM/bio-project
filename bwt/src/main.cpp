#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <random>

#include "cigar.h"
#include "util.h"
#include "bwt.cpp"

// TODO make sure we are using O3

// TODO problems when spaces and speciale characters in text
// then eliminate strings (but is useful in cigar)
// make implementation which takes only the letters in genomes
// TODO store c and o tables to files
using namespace std;


void generateSigmaFromString(const string x, map<int, int> &indexToCharIndex, vector<int> &charIndexToIndex) {
    int count = 0;
    for (char a : x) {
        if (indexToCharIndex.find(a) == indexToCharIndex.end()) {
            int charIndex = charToIndex(a);
            indexToCharIndex[charIndex] = count;
            charIndexToIndex.push_back(charIndex);
            count++;
        }
    }
}

bool compare_suffixes_indexes(const string &p, unsigned int n, int i, int j) {
    while (i < n && j < n && p[i] == p[j]) {
        i++;
        j++;
    }
    if (i < n && j < n) {
        return charToIndex(p[i]) < charToIndex(p[j]);
    } else {
        return j < n;
    }
}
// TODO all references to vector<int> should be references

/**
* construct c and o table from given string
* @param c
* @param o
* @param sa
* @param x
*/
void
construct_c_and_o(const unsigned int xn, unsigned int *c, const string &filename, const unsigned int *sa,
                  const char *x,
                  const int &step,
                  map<int, int> &charIndexToIndex,
                  vector<int> &indexToCharIndex) {
    int sigmaSize = indexToCharIndex.size();
    auto *counts = new unsigned int[sigmaSize];

    for (unsigned int i = 0; i < sigmaSize; i++) {
        counts[i] = 0;
    }

    for (unsigned int j = 0; j < xn; j++) {
        char a = x[j];
        int i = charToIndex(a);
        int &type = charIndexToIndex[i];
        counts[type]++;
    }

    // TODO should not be needed
    counts[0] = 0;

    for (unsigned int i = 1; i < sigmaSize; i++) {
        c[i] = counts[i - 1] + c[i - 1];
    }

    ofstream fout(filename + ".o", ios::out | ios::binary);

    // init o
    unsigned int temp = 0, prev = 0;
    for (unsigned int i = 0; i < sigmaSize; i++) {
        char aChar = indexToChar(indexToCharIndex[i]);
        for (unsigned int j = 0; j < xn; j++) {
            if (j == 0) {
                prev = (x[sa[0] - 1] == aChar) ? 1 : 0;
                fout.write(reinterpret_cast<const char *>(&prev), sizeof(unsigned int));
            } else {
                // when we hit the smallest suffix (there are no ones smaller than this anyway)
                bool shouldWrite = j % step == 0;
                if (sa[j] == 0) {
                    if (shouldWrite) {
                        fout.write(reinterpret_cast<const char *>(&prev), sizeof(unsigned int));
                    }
                } else {
                    temp = x[sa[j] - 1] == aChar ? 1 + prev : prev;

                    if (shouldWrite) {
                        fout.write(reinterpret_cast<const char *>(&temp), sizeof(unsigned int));
                    }
                    prev = temp;
                }
            }
        }
    }
    fout.close();
}

// call BWT_search with l = 0 and r = x.size() - 1 and w_index = w.size
vector<unsigned int> construct_suffix_array(const string &p) {
    unsigned int n = p.size();
    // TODO could be passe
    vector<unsigned int> suffixes(n);

    for (unsigned int i = 0; i < n; i++) {
        suffixes[i] = i;
    }

    auto cmp_func = [&](unsigned int i, unsigned int j) {
        return compare_suffixes_indexes(p, p.size(), i, j);
    };

    sort(suffixes.begin(), suffixes.end(), cmp_func);

    return suffixes;
}

void print_result(vector<unsigned int> results, const string &p) {
    for (auto i :results) {
        cout << (i - 1) << " " << p.size() << "M" << endl;
    }
}

/**
 * read file from string
 * @param file_name
 * @return
 */
string read_file(string file_name) {
    ifstream infile;
    infile.open(file_name);

    if (!infile.is_open()) {
        cout << "unable to open file " << file_name << endl;
        return "";
    }

    stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    return buffer.str();
}
//
//string random_string(unsigned int length, string sigma) {
//    string str(length + 1, 'x');
//
//    for (unsigned int i = 0; i < length; i++) {
//        unsigned long i1 = rand() % sigma.size();
//        str[i] = sigma[i1];
//    }
//    boost::random::mt19937 rng;         // produces randomness out of thin air
//    // see pseudo-random number generators
//    boost::random::uniform_int_distribution<> six(1,6);
//    // distribution that maps to 1..6
//    // see random number distributions
//    int x = six(rng);                   // simulate rolling a die
//
//
//    str[length] = '$';
//    return str;
//}

// should be read as pointer array
void write_unsigned_int_vector(const vector<unsigned int> &array, const string &filename) {
    // the first 8 characters is the number of elements
    unsigned int array_size = array.size();
    ofstream fout(filename, ios::out | ios::binary);
    fout.write(reinterpret_cast<const char *>(&array[0]), array.size() * sizeof(unsigned int));
    fout.close();
}


void write_unsigned_int_array(const unsigned int *array, const int n, const string &filename) {
    ofstream fout(filename, ios::out | ios::binary);
    fout.write(reinterpret_cast<const char *>(&array[0]), n * sizeof(unsigned int));
    fout.close();
}

void write_string(const char *x, const int n, const string &filename) {
    // the first 8 characters is the number of elements
    ofstream fout(filename, ios::out | ios::binary);
    fout.write(reinterpret_cast<const char *>(&x[0]), n * sizeof(char));
    fout.close();
}

unsigned int *read_1d_array_mmap(const string &filename) {
    struct stat st;
    stat(filename.c_str(), &st);
    size_t filesize = st.st_size;

    int fd = open(filename.c_str(), O_RDONLY, 0);
    auto *array = (unsigned int *) mmap(NULL, filesize, PROT_READ,
                                        MAP_PRIVATE /*| MAP_POPULATE*/, fd, 0);
    // assert(mmappedData != MAP_FAILED);
    return array;
}

int *read_1d_int_array_mmap(const string &filename) {
    struct stat st;
    stat(filename.c_str(), &st);
    size_t filesize = st.st_size;

    int fd = open(filename.c_str(), O_RDONLY, 0);
    auto *array = (int *) mmap(NULL, filesize, PROT_READ,
                               MAP_PRIVATE /*| MAP_POPULATE*/, fd, 0);
    // assert(mmappedData != MAP_FAILED);
    return array;
}

char *read_string_mmap(const string &filename, unsigned int *n) {
    struct stat st;
    stat(filename.c_str(), &st);
    size_t filesize = st.st_size;
    *n = (unsigned int) filesize;

    int fd = open(filename.c_str(), O_RDONLY, 0);
    auto *str = (char *) mmap(NULL, filesize, PROT_READ,
                              MAP_PRIVATE /*| MAP_POPULATE*/, fd, 0);
    // assert(mmappedData != MAP_FAILED);
    return str;
}


int main(int argc, char *argv[]) {
    vector<char> flags;
    map<char, string> arguments;

    // process flags
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            arguments[argv[i][1]] = argv[i + 1];
        }
    }

    map<int, int> charIndexToIndex;
    vector<int> indexToCharIndex;

    string alphabet;
    if (arguments.find('a') != arguments.end()) {
        // assume that they are sorted
        alphabet = "$" + arguments['a'];
    } else {
        alphabet = "$acgt";
    }

    generateSigmaFromString(alphabet, charIndexToIndex, indexToCharIndex);
    // remember to preserve ordering

    string filename;

    if (arguments.find('f') != arguments.end()) {
        filename = arguments['f'];
    } else {
        cout << " no argument for f " << endl;
        return 1;
    }

    int d = 0;
    if (arguments.find('d') != arguments.end()) {
        d = stoi(arguments['d']);
    }

    int step = 1;

    if (arguments.find('s') != arguments.end()) {
        step = stoi(arguments['s']);
    }

    if (step < 1) {
        cout << "step cannot be below 1" << endl;
        return 1;
    }


    // preprocess
    if (arguments.find('b') != arguments.end()) {

        if (arguments.find('p') != arguments.end()) {
            // give a pattern which exists in the file f of length p
            // the length
            unsigned int length = stoi(arguments['p']);

            unsigned int k = 0;
            unsigned int *n = &k;//length of the string
            char *x = read_string_mmap(filename + ".x", n);

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0, (*n - length));

            unsigned int start = dist(mt);
            for (unsigned int i = start; i < start + length; i++) {
                cout << x[i];
            }

            return 0;
        }

        // if g (generate) flag is set, a random string of length l is generated
        // and it's suffix array is build as well (since that is the bottle neck in this setup)
        if (arguments.find('g') != arguments.end()) {
            if (arguments.find('l') == arguments.end()) {
                cout << "No length for generated string defined" << endl;
                return 1;
            }
            // generate
            string len = arguments['l'];
            unsigned int length = atoi(len.c_str());
//
//            string x = random_string(length, alphabet.substr(1, alphabet.size() - 1));
//
//            // write string to file
//            write_string(x.c_str(), x.size(), filename + ".x");

            cout << "wrote string to file" << endl;

//            cout << "constructing sa" << endl;
//            vector<unsigned int> suffix = construct_suffix_array(x);
//            write_unsigned_int_vector(suffix, filename + ".sa");

            return 0;
        }

        auto *c = new unsigned int[indexToCharIndex.size()];
        for (int i = 0; i < indexToCharIndex.size(); i++) {
            c[i] = 0;
        }

        unsigned int *sa = read_1d_array_mmap(filename + ".sa");

        unsigned int k = 0;
        unsigned int *n = &k;//length of the string
        char *x = read_string_mmap(filename + ".x", n);

        unsigned long lengthO = ((*n + (step - 1)) / step) * indexToCharIndex.size();

        cout << "constructing c and o...";
        construct_c_and_o(*n, c, filename, sa, x, step, charIndexToIndex, indexToCharIndex);
        cout << "done" << endl;
        cout << "writing to files...";
        write_unsigned_int_array(c, indexToCharIndex.size(), filename + ".c");
        cout << "done" << endl;

        // write o table to file
        return 0;
    }

    if (arguments.find('q') != arguments.end()) {
    } else {
        cout << " not argument for q " << endl;
        return 1;
    }

    if (arguments.find('r') != arguments.end()) { // run algorithm from files constructed in b
        // .. build and run
        unsigned int *sa = read_1d_array_mmap(filename + ".sa");
        unsigned int k = 0;
        unsigned int *n = &k;//length of the string
        unsigned int kk = 0;
        unsigned int *m = &kk;//length of the string
        char *x = read_string_mmap(filename + ".x", n);

        unsigned int *c = read_1d_array_mmap(filename + ".c");
        unsigned int *o = read_1d_array_mmap(filename + ".o");

        const char *p;

        if (arguments['q'] == "file") {
            p = read_string_mmap(filename + ".q", m);
        } else {
            p = arguments['q'].c_str();
            kk = arguments['q'].size();
        }

        if (arguments.find('d') != arguments.end()) {
            vector<char> path;
            // search with edit prints the results it self..
            BWT_search_with_edit(*n, sa, x, p, c, o, step, charIndexToIndex,
                                 indexToCharIndex, d, ((*m) - 1), 0, *n - 1, path);

        } else {
            const vector<unsigned int> &results = BWT_search(*n, sa, x, p, *m, c, o, step, charIndexToIndex,
                                                             indexToCharIndex);

            if (results.empty()) {
                cout << "No results found!!!" << endl;
                return 1;
            }
            print_result(results, p);
        }
    }
}
