#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T>
typename T::iterator easyfind(T &container, const int &num) {
  typename T::iterator it = std::find(container.begin(), container.end(), num);
  if (it == container.end()) {
    throw std::runtime_error("sa");
  }
  return it;
}
