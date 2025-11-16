// @file topk_words.cpp
// Read files and prints top k word by frequency
#include <fstream>
#include <iostream>
#include <chrono>

#include "functions.h"

const size_t TOPK = 10; // Количество самых частых слов для вывода

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();
    Counter freq_dict;
    for (int i = 1; i < argc; ++i) {
        std::ifstream input{argv[i]};
        std::cout << "Processing file: " << argv[i] << '\n';
        if (!input.is_open()) {
            std::cerr << "Failed to open file " << argv[i] << '\n';
            return EXIT_FAILURE;
        }
        count_words(input, freq_dict);
    }

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}

