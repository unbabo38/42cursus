#ifndef FRAGTRAP_HPP
#define FRAGTRAP_HPP

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"

class FragTrap : public virtual ClapTrap {
public:
    FragTrap();
	FragTrap(const std::string& name);
    FragTrap(const FragTrap& other);
    FragTrap& operator=(const FragTrap& other);
    virtual ~FragTrap();

	void attack(const std::string& target);
	void beRepaired(unsigned int amount);
	void takeDamage(unsigned int amount);

	void highFivesGuys(void);
};

#endif
