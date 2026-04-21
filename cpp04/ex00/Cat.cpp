#include "Cat.hpp"

Cat::Cat() {
  this->type = "Cat";
  std::cout << "cat is born !" << std::endl;
}

Cat::Cat(const Cat& other) {
	*this = other;
}

Cat& Cat::operator=(const Cat& other) {
  if (this != &other)  {
	this->type = other.type;
  }
  return *this;
}

Cat::~Cat() {
  std::cout << type << " is dead." << std::endl;
}

void Cat::makeSound() const {
  std::cout << "meow" << std::endl;
}

std::string Cat::getType(void) const {
  return this->type;
}
