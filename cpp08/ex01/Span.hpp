#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
class Span {
private:
  unsigned int _max_size;
  std::vector<int> _nums;

public:
  Span(unsigned int n);
  ~Span();
  void addNumber(unsigned int n);
  unsigned int  shortestSpan();
  unsigned int  longestSpan();
  template <typename InputIterator>
  void addNumbers(InputIterator begin, InputIterator end) {
    if (this->_nums.size() + std::distance(begin, end) > this->_max_size) {
      throw std::out_of_range("span will be full after adding these numbers!");
    }
    this->_nums.insert(this->_nums.end(), begin, end);
  }
};
