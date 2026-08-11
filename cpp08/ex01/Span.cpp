#include "Span.hpp"

Span::Span(unsigned int n) : _max_size(n){};
Span::~Span(){};

void Span::addNumber(unsigned int n) {
  if (this->_nums.size() >= this->_max_size)
    throw std::out_of_range("span is full!");
  this->_nums.push_back(n);
};

unsigned int Span::shortestSpan() {
  if (this->_nums.size() <= 1) {
    throw std::logic_error("not enough element");
  }
  std::sort(this->_nums.begin(), this->_nums.end());
  unsigned int shortestspan = this->_nums[1] - this->_nums[0];
  for (unsigned int i = 0; i < this->_nums.size() - 1; i++) {
    unsigned int nowspan = this->_nums[i + 1] - this->_nums[i];
    if (nowspan < shortestspan)
      shortestspan = nowspan;
  }
  return shortestspan;
}

unsigned int Span::longestSpan() {
  if (this->_nums.size() <= 1) {
    throw std::logic_error("not enough element");
  }
  std::sort(this->_nums.begin(), this->_nums.end());
  return this->_nums.back() - this->_nums.front();
}
