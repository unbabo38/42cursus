#include "AMateria.hpp"

AMateria::AMateria() : type("DefaultMateria") {
  std::cout << "type constructor called." << std::endl;
}

AMateria::AMateria(std::string const &type) : type(type) {
  std::cout << "type constructor called." << std::endl;
}

AMateria::AMateria(const AMateria &materia) {
  *this = materia;
  std::cout << "type copy constructor called." << std::endl;
}

AMateria &AMateria::operator=(const AMateria &materia) {
  if (this != &materia)
    this->type = materia.type;
  std::cout << "type copy constructor called." << std::endl;
  return *this;
}

AMateria::~AMateria() { std::cout << "type destructor called" << std::endl; }

std::string const &AMateria::getType() const { return this->type; }

void AMateria::use(ICharacter &target) { (void)target; }
