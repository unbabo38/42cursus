#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include <array>

class Brain {
  public:
	std::array <std::string, 100> ideas;

	Brain();
	Brain(const Brain& other);
	Brain& operator=(const Brain &other);
	virtual ~Brain();
};

#endif
