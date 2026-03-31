#include "Zombie.hpp"

// Constructor
Zombie::Zombie(std::string name) : _name(name)
{
	std::cout << _name << " is coming...\n" << std::endl;
}

// Deconstructor
Zombie::~Zombie()
{
	std::cout << _name << " died.\n"  << std::endl;
}

// Public Methods
void	Zombie::announce(void) const
{
	std::cout << _name << ": BraiiiiiiinnnzzzZ...\n" << std::endl;
}
