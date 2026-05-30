
#include "iter.hpp"

void print(int a) { printf("int= %d\n", a); }

int main() {
  int array[4];
  array[0] = 1;
  array[1] = 2;
  array[2] = 3;
  array[3] = 4;

  iter(array, 4, print);
}
