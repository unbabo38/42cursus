#include "Zombie.hpp"
#include <iostream>

// デフォルトコンストラクタ (new[] の時に呼ばれる)
Zombie::Zombie() : _name("") {
    // コンピュータ視点：中身を空にして場所だけ確保
}

// デストラクタ (delete[] の時に呼ばれる)
Zombie::~Zombie() {
    std::cout << "Zombie " << this->_name << " is destroyed." << std::endl;
}

void Zombie::announce() {
    std::cout << this->_name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

void Zombie::setName(std::string name) {
    this->_name = name;
}
