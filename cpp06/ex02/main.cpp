#include "Base.hpp"
#include <iostream>
#include <ctime>

int main() {
    // 実行するたびにランダムな結果が変わるように、乱数の種を植えるっぴ
    std::srand(std::time(NULL));

    std::cout << "=== Test 1 ===" << std::endl;
    Base *ptr1 = generate();

    std::cout << "--- Pointer Identify ---" << std::endl;
    identify(ptr1);

    std::cout << "--- Reference Identify ---" << std::endl;
    identify(*ptr1); // ポインタに * をつけて参照として渡すっぴ！

    delete ptr1; // メモリリーク防止！

    std::cout << "\n=== Test 2 ===" << std::endl;
    Base *ptr2 = generate();
    identify(ptr2);
    identify(*ptr2);
    delete ptr2;

    return 0;
}
