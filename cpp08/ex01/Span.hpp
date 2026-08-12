#include <algorithm>
#include <iostream>
#include <vector>
class Span {
private:
  unsigned int _max_size;
  std::vector<int> _nums;

public:
  Span(unsigned int n);
  Span(const Span &other);
  Span &operator=(const Span &other);
  ~Span();
  void addNumber(const int n);
  unsigned int  shortestSpan();
  unsigned int  longestSpan();
  std::vector<int> getNums() const;
};
