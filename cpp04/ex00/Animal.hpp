#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class Animal {
  protected:
	std::string type;

  public:
    Animal();
	Animal(const std::string type);
	Animal(const Animal& other);
	Animal& operator=(const Animal& other);
	virtual ~Animal();

	void virtual makeSound() const;
	std::string virtual getType(void) const;
};

#endif
