#include <iostream>

template <typename T> class Array {
private:
  T *_array;
  unsigned int _size;

public:
  Array() : _array(NULL), _size(0) {}

  Array(const Array &other) : _array(NULL), _size(0) { *this = other; }

  Array &operator=(const Array &other) {
    if (this != &other) {
      delete[] this->_array;
      this->_size = other._size;
      if (this->_size > 0) {
        this->_array = new T[this->_size]();
        for (unsigned int i = 0; i < this->_size; i++) {
          this->_array[i] = other._array[i];
        }
      } else {
        this->_array = NULL;
      }
    }
    return *this;
  }
  Array(unsigned int n) : _size(n) {
    if (n == 0) {
      _array = NULL;
    } else {
      _array = new T[n]();
    }
  }
  ~Array() { delete[] this->_array; }
  unsigned int size() const { return this->_size; }

  T &operator[](unsigned int i) {
    if (i >= this->_size) {
      throw std::out_of_range("Array: index out of range");
    }
    return this->_array[i];
  }

  const T &operator[](unsigned int i) const {
    if (i >= this->_size) {
      throw std::out_of_range("Array: index out of range");
    }
    return this->_array[i];
  }
};
