#include "Zombie.hpp"
#include <iostream>

int main()
{
    int N = 5;

    // 1. 軍団の召喚
    Zombie* horde = zombieHorde(N, "HordeMember");
    if (!horde) return 1;

    // 2. 全員の生存確認
    for (int i = 0; i < N; i++) {
        std::cout << i << ": ";
        horde[i].announce();
    }

    // 3. 一括解散（delete[] を忘れるとメモリリーク！）
    // コンピュータ視点：
    // 「おっ、[] が付いてるな。じゃあ N 回デストラクタを回してから、
    // まとめてメモリを OS に返却するぞ」
    delete[] horde;

    return 0;
}
