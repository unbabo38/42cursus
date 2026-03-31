#include <string>

class Zombie {
private:
    std::string _name;
public:
    Zombie(); // new[] のために必須！
    ~Zombie();
    void announce();
    void setName(std::string name); // 後から名前を付ける用
};
Zombie* zombieHorde( int N, std::string name );
