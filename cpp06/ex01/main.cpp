#include "Serializer.hpp"
#include <iostream>

int main() {
    Data value;
    value.k = 42; // C++98なのでここで初期化

    std::cout << "--- Original Pointer ---" << std::endl;
    std::cout << "Address of value : " << &value << std::endl;
    std::cout << "Value of value.k : " << value.k << std::endl;
    std::cout << std::endl;

    // 1. ポインタを整数に変換（シリアライズ）
    uintptr_t raw = Serializer::serialize(&value);
    std::cout << "--- Serialized (uintptr_t) ---" << std::endl;
    std::cout << "uintptr_t value  : " << raw << std::endl;
    std::cout << std::endl;

    // 2. 整数をポインタに復元（デシリアライズ）
    Data* restored_ptr = Serializer::deserialize(raw);
    std::cout << "--- Deserialized Pointer ---" << std::endl;
    std::cout << "Restored Address : " << restored_ptr << std::endl;
    std::cout << "Restored value.k : " << restored_ptr->k << std::endl;
    std::cout << std::endl;

    // 3. アドレスが完全に一致するか検証
    std::cout << "--- Verification ---" << std::endl;
    if (restored_ptr == &value) {
        std::cout << "SUCCESS: Pointers match perfectly!" << std::endl;
    } else {
        std::cout << "FAILURE: Pointers do not match!" << std::endl;
    }

    return 0;
}
