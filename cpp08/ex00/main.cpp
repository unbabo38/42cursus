#include "easyfind.hpp"

int main() {
  std::vector<int> n(10);
  n[3] = 12;
  try {
    easyfind(n, 3);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
