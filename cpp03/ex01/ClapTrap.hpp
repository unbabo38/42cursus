#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>
#include <string>
#include <climits>
#include <iomanip>

class ClapTrap {
  protected:
	std::string					_name;
	unsigned int  				_hitPoints;
	unsigned int				_energyPoints;
	unsigned int				_attackDamage;

  public:
	ClapTrap();
	ClapTrap(const std::string& name);
	ClapTrap(const ClapTrap& other);
	ClapTrap& operator=(const ClapTrap& other);
	~ClapTrap();

	void 				attack(const std::string& target);
	void virtual 		takeDamage(unsigned int amount);
	void virtual		beRepaired(unsigned int amount);
	void				printStatus(void) const;
};

#endif
