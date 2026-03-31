#ifndef WEAPON_HPP
# define WEAPON_HPP

# include <string>

class Weapon {
private:
    std::string type;
public:
    Weapon(std::string type);
    ~Weapon();
    const std::string& getType() const; // 戻り値はstringの定数参照
    void setType(std::string newType);
};

#endif
