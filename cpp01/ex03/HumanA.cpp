#include "HumanA.hpp"

// 参照メンバは「初期化リスト」でしか初期化できない
HumanA::HumanA(std::string name, Weapon& weapon) : _name(name), _weapon(weapon) {}
HumanA::~HumanA() {}

void HumanA::attack() const {
    std::cout << _name << " attacks with their " << _weapon.getType() << std::endl;
}
