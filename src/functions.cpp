// @file functions.cpp
#include "functions.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

void count_words(std::istream& stream, Counter& counter) {
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string &s) { ++counter[tolower(s)]; });    
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
        std::begin(words), std::begin(words) + k, std::end(words),
        [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
        std::begin(words), std::begin(words) + k,
        [&stream](const Counter::const_iterator &pair) {
            stream << std::setw(4) << pair->second << " " << pair->first
                      << '\n';
        });
}

void process_origin(int argc, char* argv[]) {
    std::cout << "\n=========================================\n";
    std::cout << "        ORIGINAL VERSION (OTUS)\n";
    std::cout << "=========================================\n";
    auto start = std::chrono::high_resolution_clock::now();
    Counter freq_dict;
    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        // std::cout << "Processing file: " << argv[i] << '\n';
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            return;
        }
        count_words(input, freq_dict);
    }

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time (original): " << elapsed_ms.count() << " us\n";
}

void process_v1(int argc, char* argv[]) {
    std::cout << "\n=========================================\n";
    std::cout << "        OPTIMIZED VERSION v1\n";
    std::cout << "=========================================\n";
    auto start = std::chrono::high_resolution_clock::now();
    Counter freq_dict;
    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        // std::cout << "Processing file: " << argv[i] << '\n';
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            return;
        }
        count_words(input, freq_dict);
    }

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time (v1): " << elapsed_ms.count() << " us\n";
}
