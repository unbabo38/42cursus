#include "Harl.hpp"

Harl::Harl() {
    // コンストラクタ（空でOK）
}

Harl::~Harl() {
    // デストラクタ（空でOK）
}

void Harl::debug(void) {
    std::cout << "[ DEBUG ]\nI love having extra bacon... I really do!" << std::endl;
}

void Harl::info(void) {
    std::cout << "[ INFO ]\nI cannot believe adding extra bacon costs more money..." << std::endl;
}

void Harl::warning(void) {
    std::cout << "[ WARNING ]\nI think I deserve to have some extra bacon for free..." << std::endl;
}

void Harl::error(void) {
    std::cout << "[ ERROR ]\nThis is unacceptable! I want to speak to the manager now." << std::endl;
}

void Harl::complain(std::string level) {
    // メンバ関数ポインタの配列を作成
    void (Harl::*functions[])(void) = {
        &Harl::debug,
        &Harl::info,
        &Harl::warning,
        &Harl::error
    };

    // 対応するレベル文字列の配列
    std::string levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

    for (int i = 0; i < 4; i++) {
        if (levels[i] == level) {
            // 見つかったらその関数を呼び出す
            (this->*functions[i])();
            return;
        }
    }
}
