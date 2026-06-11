
#include "iter.hpp"


void displaystr(const std::string &str) {
  std::cout << str << std::endl;
}

void displayint(const int &num) {
  std::cout << num << std::endl;
}

void displaychar(const char &c) {
  std::cout << c << std::endl;
}

void displaydouble(const double &dbl) {
  std::cout << dbl << std::endl;
}

void displayfloat(const float &flt) {
  std::cout << flt << std::endl;
}

template <typename T, typename F>
void iter(T *array, const std::size_t length, F func) {
  if (array == NULL || func == NULL)
    return;
  for (std::size_t i = 0; i < length; i++) {
    (*func)(array[i]);
  }
}

int main() {
  int intarray[4];
  intarray[0] = 1;
  intarray[1] = 2;
  intarray[2] = 3;
  intarray[3] = 4;

  iter(intarray, 4, displayint);

  std::string strarray[4];
  strarray[0] = "abcdefg";
  strarray[1] = "hijklmn";
  strarray[2] = "opqrstu";
  strarray[3] = "vwxyz";

  iter(strarray, 4, displaystr);

  char chararray[4];
  chararray[0] = 'a';
  chararray[1] = 'b';
  chararray[2] = 'c';
  chararray[3] = 'd';

  iter(chararray, 4, displaychar);

  float farray[4];
  farray[0] = 1.1;
  farray[1] = 2.2;
  farray[2] = 3.3;
  farray[3] = 4.4;

  iter(farray, 4, displayfloat);

  double darray[4];
  darray[0] = 1.1;
  darray[1] = 2.2;
  darray[2] = 3.3;
  darray[3] = 4.4;

  iter(darray, 4, displaydouble);
}
