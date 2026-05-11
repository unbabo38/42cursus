#include "Dog.hpp"

Dog::Dog() : Animal() {
  this->type = "Dog";
  this->brain = new Brain();
  std::cout << "Dog is born!" << std::endl;
}

Dog::Dog(const Dog &other) : Animal(other) {
  this->brain = new Brain();
  *this = other;
}

Dog &Dog::operator=(const Dog &other) {
  if (this == &other)
    return *this;
  this->type = other.type;
  if (this->brain) {
    delete this->brain;
    this->brain = new Brain(*other.brain);
  }
  return *this;
}

Dog::~Dog() {
  delete this->brain;
  std::cout << "Dog is dead." << std::endl;
}

void Dog::makeSound() const { std::cout << "wan!" << std::endl; }

std::string Dog::getType(void) const { return this->type; }

std::string Dog::getIdea(int index) const { return this->brain->ideas[index]; }

void Dog::setIdea(int index, const std::string idea) {
  this->brain->ideas[index] = idea;
}
