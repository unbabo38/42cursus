#ifndef EASYFIND_HPP
#define EASYFIND_HPP
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

template <typename T>
typename T::iterator easyfind(T &container, const int &num);

#include "easyfind.tpp"

#endif
