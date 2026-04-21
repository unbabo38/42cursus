#ifndef DOG_HPP
#define DOG_HPP

#include <iostream>
#include <string>
#include "Animal.hpp"

class Dog : public virtual Animal{
  public:
    Dog();
	Dog(const std::string type);
	Dog(const Dog& other);
	Dog& operator=(const Dog& other);
	virtual ~Dog();

	void makeSound() const;
	std::string getType(void) const;
};

#endif
