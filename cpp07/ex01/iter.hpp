#include <cstddef>
#include <string>
#include <iostream>

template <typename T, typename F>
void iter(T *array, const std::size_t length, F func);
void displayint(const int &num);
void displaystr(const std::string &info);
void displaydouble(const double &dbr);
void displayfloat(const float &flt);
void displaychar(const char &c);
