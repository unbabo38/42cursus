#include "Dog.hpp"

Dog::Dog() {
  this->type = "Dog";
  std::cout << "Dog is born !" << std::endl;
}

Dog::Dog(const Dog& other) {
	*this = other;
}

Dog& Dog::operator=(const Dog& other) {
  if (this != &other)  {
	this->type = other.type;
  }
  return *this;
}

Dog::~Dog() {
  std::cout << type << " is dead." << std::endl;
}

void Dog::makeSound() const {
  std::cout << "vowu" << std::endl;
}

std::string Dog::getType(void) const {
  return this->type;
}
