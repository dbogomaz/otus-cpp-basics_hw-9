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
#include <mutex>
#include <thread>

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
    std::cout << "\n==================================================\n";
    std::cout << "             ORIGINAL VERSION (OTUS)\n";
    std::cout << "==================================================\n";
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
    std::cout << "\n==================================================\n";
    std::cout << "             OPTIMIZED VERSION v1\n";
    std::cout << "Каждая книга в отдельном потоке. Словарь общий\n";
    std::cout << "==================================================\n";
    auto start = std::chrono::high_resolution_clock::now();

    Counter freq_dict; // словарь будет общий
    std::mutex freq_dict_mutex; // защита словаря

    int file_count = argc - 1;
    std::vector<std::thread> workers; // вектор потоков
    workers.reserve(file_count);

    for (int i = 1; i < argc; ++i) {
        char *filename = argv[i];

        workers.emplace_back([filename, &freq_dict, &freq_dict_mutex]() {
            std::ifstream input{filename};
            if (!input.is_open()) {
                std::cerr << "Failed to open file " << filename << '\n';
                return;
            }
            Counter local_counter; // словарь на текущий поток
            count_words(input, local_counter);

            std::lock_guard<std::mutex> lock(freq_dict_mutex);
            for (const auto& [word, count] : local_counter) {
                freq_dict[word] += count;
            }
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time (v1): " << elapsed_ms.count() << " us\n";
}

void process_v2(int argc, char* argv[]) {
    std::cout << "\n==================================================\n";
    std::cout << "             OPTIMIZED VERSION v2\n";
    std::cout << "Каждая книга в отдельном потоке. Словарь у каждого\n";
    std::cout << "потока свой. Потом словари объединяются\n";
    std::cout << "==================================================\n";
    auto start = std::chrono::high_resolution_clock::now();

    int file_count = argc - 1;
    std::vector<Counter> local_dicts(file_count); // Вектор локальных словарей (по одному на поток)

    std::vector<std::thread> workers; // вектор потоков
    workers.reserve(file_count); // в v1 тоже заререзвировать

    for (int i = 1; i < argc; ++i) {
        char *filename = argv[i];
        int index = i - 1; // 

        workers.emplace_back([filename, index, &local_dicts]() {
            std::ifstream input{filename};
            if (!input.is_open()) {
                std::cerr << "Failed to open file " << filename << '\n';
                return;
            }
            count_words(input, local_dicts[index]);
        });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    // --- Теперь объединяем ВСЕ локальные словари ---
    Counter final_dict;

    for (const auto& local : local_dicts) {
        for (const auto& [word, count] : local) {
            final_dict[word] += count;
        }
    }

    print_topk(std::cout, final_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time (v2): " << elapsed_ms.count() << " us\n";
}
