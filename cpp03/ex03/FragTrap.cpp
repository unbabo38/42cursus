#include "FragTrap.hpp"

FragTrap::FragTrap() : ClapTrap() {
    this->_hitPoints = 100;
    this->_energyPoints = 100;
    this->_attackDamage = 30;
    std::cout << "FragTrap " << this->_name << " is ready!" << std::endl;
	printStatus();

}

FragTrap::FragTrap(const std::string& name) : ClapTrap(name) {
    std::cout << "FragTrap " << this->_name << " is ready!" << std::endl;
    this->_hitPoints = 100;
    this->_energyPoints = 100;
    this->_attackDamage = 30;
	printStatus();
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other) {
    std::cout << "FragTrap Copy constructor called" << std::endl;
    *this = other;
	printStatus();
}

FragTrap& FragTrap::operator=(const FragTrap& other) {
	std::cout << "FragTrap assignment operator called" << std::endl;
	if (this != &other) {
		this->_name = other._name;
		this->_hitPoints = other._hitPoints;
		this->_energyPoints = other._energyPoints;
		this->_attackDamage = other._attackDamage;
	}
	return *this;
}

FragTrap::~FragTrap() {
    std::cout << "FragTrap " << _name << " deactivated." << std::endl;
}

void  FragTrap::attack(const std::string& target){
  if (_hitPoints <= 0 || _energyPoints <= 0) {
	std::cout << "FragTrap " << _name << " has no points left to attack" << std::endl;
	return ;
  }
  _energyPoints--;
  std::cout << "FragTrap " << _name << " attacks " << target << " ,causing " << _attackDamage << " points of damage!" << std::endl;
  printStatus();
}

void  FragTrap::takeDamage(unsigned int amount) {
  if (amount > static_cast<unsigned int>(_hitPoints)) {
	std::cout << "FragTrap " << _name << " takes " << _hitPoints << " damage!" << std::endl;
	std::cout << "FragTrap " << _name << " has no hit points" << std::endl;
	_hitPoints = 0;
	printStatus();
  }
  else
  {
	_hitPoints -= amount;
  	std::cout << "FragTrap " << _name << " takes " << amount << " damage!" << std::endl;
	printStatus();
  }
}

void FragTrap::beRepaired(unsigned int amount) {
    if (_hitPoints <= 0 || _energyPoints <= 0) {
        std::cout << "FragTrap " << _name << " can't repair!" << std::endl;
        return;
    }
	// 最大値までの残り空き容量よりも回復させたい量の方が多い場合、空き容量の分だけ回復,そうでなければ、指定された量だけ回復
	unsigned int actualAmount = (amount > UINT_MAX - _hitPoints) ? (UINT_MAX - _hitPoints) : amount;

    _energyPoints -= 1;
	if (amount > UINT_MAX - _hitPoints)
		_hitPoints = UINT_MAX;
	else
	    _hitPoints += amount;
    std::cout << "FragTrap " << _name << " repairs " << actualAmount << " points!" << std::endl;
	printStatus();
}

void FragTrap::highFivesGuys(void) {
	if (_hitPoints <= 0 || _energyPoints <= 0) {
        std::cout << "FlapTrap " << _name << " can't say hi..." << std::endl;
        return;
    }
    std::cout << "FragTrap " << _name << " says: High Five, guys!" << std::endl;
}
