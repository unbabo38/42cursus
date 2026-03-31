#include "Zombie.hpp"

Zombie* zombieHorde(int N, std::string name)
{
    if (N <= 0) return NULL;

    // コンピュータ視点：メモリ上に N 体分の連続した領域を確保
    // この時、全てのゾンビのデフォルトコンストラクタが呼ばれる
    Zombie* horde = new Zombie[N];

    // 各ゾンビに順番にアクセスして名前を刻み込む
    for (int i = 0; i < N; i++) {
        horde[i].setName(name);
    }

    return horde; // 0番目のゾンビの住所を返す
}
