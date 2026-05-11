#include <iostream>

class MyDevice {
public:
    MyDevice() {}
    // コピー禁止（ostreamと同じ設定）
    MyDevice(const MyDevice&) = delete;

    // 参照を返せば、連鎖できる
    MyDevice& operator<<(const char* msg) {
        std::cout << msg << std::endl;
        return *this;
    }

     // もし値で返そうとすると...
    // MyDevice operator<<(int n) {
    //     return *this; // ここで「コピー禁止」に引っかかってエラー！
    // }

};

int main() {
    MyDevice dev;
    dev << "Hello" << "World"; // 参照返しなら成功
}
