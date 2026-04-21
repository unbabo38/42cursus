#ifndef CLAPTRAP_HPP
#define CLAPTRAP_HPP

#include <iostream>
#include <string>
#include <climits>
#include <iomanip>

class ClapTrap {
  private:
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

	void 						attack(const std::string& target);
	void 						takeDamage(unsigned int amount);
	void 						beRepaired(unsigned int amount);
	void						printStatus(void) const;
};

#endif
