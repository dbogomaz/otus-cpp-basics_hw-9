#include <benchmark/benchmark.h>

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "functions.h"

// ----------------------------------------------------------
// Список книг из папки ./books/
// ----------------------------------------------------------
static std::vector<std::string> get_books_list() {
    std::vector<std::string> books;
    for (const auto& entry : std::filesystem::directory_iterator("books")) {
        books.emplace_back(entry.path().string());
    }
    return books;
}

static const std::vector<std::string> g_books = get_books_list();

// ----------------------------------------------------------
// Генерация argv
// ----------------------------------------------------------
static std::vector<char*> make_argv() {
    std::vector<char*> argv;
    argv.reserve(g_books.size() + 1);

    // argv[0] — фиктивное имя программы
    static std::string app_name = "bench_topk";
    argv.push_back(app_name.data());

    // argv[1..N] — пути к книгам
    for (auto& file : g_books) {
        argv.push_back(const_cast<char*>(file.c_str()));
    }

    return argv;
}

// ----------------------------------------------------------
//  Бенчмарк оригинальной реализации OTUS
// ----------------------------------------------------------
static void BM_ProcessOrigin(benchmark::State& state) {
    auto argv_vec = make_argv();
    int argc = (int)argv_vec.size();
    char** argv = argv_vec.data();

    for (auto _ : state) {
        // Перенаправляем вывод в "никуда"
        std::ostringstream null_output;
        std::streambuf* old = std::cout.rdbuf(null_output.rdbuf());

        process_origin(argc, argv);

        std::cout.rdbuf(old);
    }
}
BENCHMARK(BM_ProcessOrigin);

// ----------------------------------------------------------
//  Вариант V1
// ----------------------------------------------------------
static void BM_ProcessV1(benchmark::State& state) {
    auto argv_vec = make_argv();
    int argc = (int)argv_vec.size();
    char** argv = argv_vec.data();

    for (auto _ : state) {
        std::ostringstream null_output;
        std::streambuf* old = std::cout.rdbuf(null_output.rdbuf());

        process_v1(argc, argv);

        std::cout.rdbuf(old);
    }
}
BENCHMARK(BM_ProcessV1);

// ----------------------------------------------------------
//  Вариант V2
// ----------------------------------------------------------
static void BM_ProcessV2(benchmark::State& state) {
    auto argv_vec = make_argv();
    int argc = (int)argv_vec.size();
    char** argv = argv_vec.data();

    for (auto _ : state) {
        std::ostringstream null_output;
        std::streambuf* old = std::cout.rdbuf(null_output.rdbuf());

        process_v2(argc, argv);

        std::cout.rdbuf(old);
    }
}
BENCHMARK(BM_ProcessV2);

BENCHMARK_MAIN();
