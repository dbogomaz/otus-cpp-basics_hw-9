// @file topk_words.cpp
// Read files and prints top k word by frequency
#include "functions.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    process_origin(argc, argv);
    process_v1(argc, argv);
    process_v2(argc, argv);

}

