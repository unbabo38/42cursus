#include "Weapon.hpp"

// × Weapon(std::string type)
// ○ Weapon::Weapon(std::string type) ← これが必要！
Weapon::Weapon(std::string type) : type(type) {}

Weapon::~Weapon() {}

const std::string& Weapon::getType() const {
    return this->type;
}

void Weapon::setType(std::string newType) {
    this->type = newType;
}
