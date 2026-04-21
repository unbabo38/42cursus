#include "Cat.hpp"

Cat::Cat() : Animal() {
    this->type = "Cat";
    this->brain = new Brain();
    std::cout << "Cat is born!" << std::endl;
}

Cat::Cat(const Cat& other) : Animal(other){
	this->brain = new Brain();
	*this = other;
}

Cat& Cat::operator=(const Cat& other) {
  if (this == &other)
    return *this;
  this->type = other.type;
  if (this->brain)
    delete this->brain;
    this->brain = new Brain(*other.brain);
  return *this;
}

Cat::~Cat() {
  delete this->brain;
  std::cout << type << " is dead." << std::endl;
}

void Cat::makeSound() const {
  std::cout << "nyaa!" << std::endl;
}

std::string Cat::getType(void) const {
  return this->type;
}

std::string Cat::getIdea(int index) const {
  return this->brain->ideas[index];
}

void Cat::setIdea(int index, const std::string idea) {
  this->brain->ideas[index] = idea;
}
