#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <map>
#include <istream>
#include <ostream>

using Counter = std::map<std::string, std::size_t>; // Словарь для подсчета слов

std::string tolower(const std::string &str); // Преобразование строки в нижний регистр

void count_words(std::istream& stream, Counter&); // Подсчет слов в потоке

void print_topk(std::ostream& stream, const Counter&, const size_t k); // Вывод топ k слов

#endif // FUNCTIONS_H