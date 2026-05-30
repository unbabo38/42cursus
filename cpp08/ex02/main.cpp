#include "MutantStack.hpp"

int main() {
  MutantStack<int> m;
  m.push(1);
  std::cout << m.top() << std::endl;
  m.pop();

  m.push(1);
  m.push(2);
  m.push(3);

  MutantStack<int>::it itbegin = m.begin();
  MutantStack<int>::it itend = m.begin();

  while (itbegin != itend) {
    std::cout << *itbegin << std::endl;
    ++itbegin;
  }
}
