#ifndef HUMANA_HPP
# define HUMANA_HPP

# include "Weapon.hpp"
# include <iostream>

class HumanA {
private:
    std::string _name;
    Weapon&     _weapon; // 参照メンバ
public:
    // 参照は初期化が必要なのでコンストラクタで受け取る
    HumanA(std::string name, Weapon& weapon);
    ~HumanA();
    void attack() const;
};

#endif
