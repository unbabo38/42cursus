#include "ScavTrap.hpp"
#include "FragTrap.hpp"
#include <climits>

int main() {
    std::cout << "========== 1. ClapTrap: 基本機能 & 境界値テスト ==========" << std::endl;
    {
        ClapTrap ct;
        ct.attack("Enemy");
        ct.takeDamage(5);
        ct.beRepaired(UINT_MAX); // 最大値テスト
        ct.takeDamage(10);
        ct.beRepaired(10);
    }

    std::cout << "\n========== 2. 継承: 生成と消滅の連鎖 (Chaining) ==========" << std::endl;
    {
        // スコープを抜けるときに 子 -> 親 の順でデストラクタが動くか確認
        std::cout << "[ ScavTrap Case ]" << std::endl;
        ScavTrap scav;
        std::cout << "[ FragTrap Case ]" << std::endl;
        FragTrap frag;
    }

    std::cout << "\n========== 3. ポリモーフィズム: 仮想関数のテスト ==========" << std::endl;
    {
        std::cout << "-- ポインタ経由での操作 --" << std::endl;
        // 親のポインタで子クラスを指す
        ClapTrap* meta[2];
        meta[0] = new ScavTrap();
        meta[1] = new FragTrap();

        // attackがvirtualなら、それぞれのクラス固有のAD(20や30)やメッセージが出る
        for (int i = 0; i < 2; i++) {
            meta[i]->attack("Common Enemy");
        }

        std::cout << "-- 仮想デストラクタの確認 --" << std::endl;
        // 親のポインタ経由でdeleteした際、正しく「子→親」の順でログが出るか
        // これが virtual ~ClapTrap() の存在意義！
        for (int i = 0; i < 2; i++) {
            delete meta[i];
        }
    }

    std::cout << "\n========== 4. 子クラス固有機能のテスト ==========" << std::endl;
    {
        ScavTrap scav;
        FragTrap frag;

        scav.guardGate();    // ScavTrap特有
        frag.highFivesGuys(); // FragTrap特有
    }

    std::cout << "\n========== 5. 異常系: HP 0 での行動制限 ==========" << std::endl;
    {
        FragTrap deadFrag;
        deadFrag.takeDamage(200); // 死亡させる
        deadFrag.attack("Someone");   // 動けないはず
        deadFrag.beRepaired(10);      // 修理できないはず
        deadFrag.highFivesGuys();     // (仕様によるが)ログが出るか確認
    }

    return 0;
}
