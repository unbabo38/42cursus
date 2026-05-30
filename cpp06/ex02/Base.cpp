#include "Base.hpp"

Base::~Base() {}

Base *generate(void) {
  int random_num = std::rand() % 3;
  if (random_num == 0) {
	A *a = new A;
	return a;
  }
  if (random_num == 1)
  {
    B *b = new B;
	return b;
  }
  if (random_num == 2) {
    C *c = new C;
	return c;
  }
  return NULL;
}

void identify(Base *p) {
  if (A *a = dynamic_cast<A *>(p)) {
	std::cout << "this is children class A" << std::endl;
  }
  else if (B *b = dynamic_cast<B *>(p)) {
	std::cout << "this is children class B" << std::endl;
  }
  else if (C *c = dynamic_cast<C *>(p)) {
	std::cout << "this is children class C" << std::endl;
  }
  else
  	std::cout << "this is not children class" << std::endl;
}

void identify(Base &p) {
    try {
        (void)dynamic_cast<A &>(p);
        std::cout << "this is children class A" << std::endl;
        return;
    }
    catch (const std::exception &e) {
        try {
            (void)dynamic_cast<B &>(p);
            std::cout << "this is children class B" << std::endl;
            return;
        } catch (const std::exception &e) {
            try {
                (void)dynamic_cast<C &>(p);
                std::cout << "this is children class C" << std::endl;
                return;
            } catch (const std::exception &e) {
                std::cout << "Unknown class: " << e.what() << std::endl;
            }
        }
    }
}
