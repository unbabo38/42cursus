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
  void addNumber(unsigned int n) {
    if (_nums.size() >= _max_size)
      throw std::out_of_range("span is full!");
    _nums.push_back(n);
  };

  int shortestSpan() {
    if (_nums.size() <= 1) {
      throw std::logic_error("not enough element");
    }
    std::sort(_nums.begin(), _nums.end());
    int shortestspan = UINT16_MAX - 1;
    for (int i = 0; i < _nums.size() - 1; i++) {
      int nowspan = _nums[i + 1] - _nums[i];
      if (nowspan < shortestspan)
        shortestspan = nowspan;
    }
    return shortestspan;
  }
  int longestSpan() {
    if (_nums.size() <= 1) {
      throw std::logic_error("not enough element");
    }
    std::sort(_nums.begin(), _nums.end());
    return _nums.back() - _nums.front();
  }

  template <typename InputIterator>
  void addNumbers(InputIterator begin, InputIterator end) {
    if (_nums.size() + std::distance(begin, end) > _max_size) {
      throw std::out_of_range("span will be full after adding these numbers!");
    }
    _nums.insert(_nums.end(), begin, end);
  }
};
