#include <cstddef>
#include <stdio.h>

template <typename T, typename F>
void iter(T *array, const std::size_t length, F func) {
  if (array == NULL || func == NULL)
    return;
  for (std::size_t i = 0; i < length; i++) {
    (*func)(array[i]);
  }
}
