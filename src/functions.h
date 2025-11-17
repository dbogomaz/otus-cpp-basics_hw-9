#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <map>
#include <istream>
#include <ostream>

const size_t TOPK = 10; // Количество самых частых слов для вывода

using Counter = std::map<std::string, std::size_t>; // Словарь для подсчета слов
std::string tolower(const std::string &str); // Преобразование строки в нижний регистр
void count_words(std::istream& stream, Counter&); // Подсчет слов в потоке
void print_topk(std::ostream& stream, const Counter&, const size_t k); // Вывод топ k слов

void process_origin(int argc, char *argv[]); // оригинальная реализация от OTUS
void process_v1(int argc, char *argv[]);

#endif // FUNCTIONS_H