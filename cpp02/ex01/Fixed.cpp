#include "Fixed.hpp"

Fixed::Fixed() : _rawBits(0) {
    std::cout << "Default constructor called" << std::endl;
}

// 整数コンストラクタ: 8ビット左シフト (n * 256)
Fixed::Fixed(const int n) {
    std::cout << "Int constructor called" << std::endl;
    this->_rawBits = n << _fractionalBits;
}

// 浮動小数点コンストラクタ: 256倍して四捨五入
Fixed::Fixed(const float f) {
    std::cout << "Float constructor called" << std::endl;
    this->_rawBits = roundf(f * (1 << _fractionalBits));
}

Fixed::Fixed(const Fixed& other) {
    std::cout << "Copy constructor called" << std::endl;
    *this = other;
}

Fixed& Fixed::operator=(const Fixed& other) {
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &other) {
        this->_rawBits = other.getRawBits();
    }
    return *this;
}

Fixed::~Fixed() {
    std::cout << "Destructor called" << std::endl;
}

// 浮動小数点に戻す: 256.0 で割る
float Fixed::toFloat(void) const {
    return (float)this->_rawBits / (1 << _fractionalBits);
}

// 整数に戻す: 8ビット右シフト
int Fixed::toInt(void) const {
    return this->_rawBits >> _fractionalBits;
}

int Fixed::getRawBits(void) const {
    return this->_rawBits;
}

void Fixed::setRawBits(int const raw) {
    this->_rawBits = raw;
}

// 出力演算子の実装
std::ostream& operator<<(std::ostream& os, const Fixed& fixed) {
    os << fixed.toFloat();
    return os;
}
