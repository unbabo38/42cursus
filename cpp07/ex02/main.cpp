#include "Array.hpp"

int main() {
  Array<int> a(5);
  a[0] = 42;
  for (int i = 0; i < 5; i++) {
    std::cout << a[i] << std::endl;
  }
  Array<int> b = a;
  b[0] = 999;

  for (int i = 0; i < 5; i++) {
    std::cout << b[i] << std::endl;
  }
}
