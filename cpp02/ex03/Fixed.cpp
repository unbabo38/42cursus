#include "Fixed.hpp"

/* --- Constructors & Destructor (Orthodox Canonical Form) --- */

Fixed::Fixed() : _rawBits(0) {}

Fixed::Fixed(const int n) {
    this->_rawBits = n << _fractionalBits;
}

Fixed::Fixed(const float f) {
    this->_rawBits = roundf(f * (1 << _fractionalBits));
}

Fixed::Fixed(const Fixed& other) {
    *this = other;
}

Fixed& Fixed::operator=(const Fixed& other) {
    if (this != &other) {
        this->_rawBits = other.getRawBits();
    }
    return *this;
}

Fixed::~Fixed() {}

/* --- Raw Bits Accessors --- */

int Fixed::getRawBits(void) const {
    return this->_rawBits;
}

void Fixed::setRawBits(int const raw) {
    this->_rawBits = raw;
}

/* --- Type Conversions --- */

float Fixed::toFloat(void) const {
    return (float)this->_rawBits / (1 << _fractionalBits);
}

int Fixed::toInt(void) const {
    return this->_rawBits >> _fractionalBits;
}

/* --- 6 Comparison Operators --- */

bool Fixed::operator>(const Fixed& other) const { return this->_rawBits > other._rawBits; }
bool Fixed::operator<(const Fixed& other) const { return this->_rawBits < other._rawBits; }
bool Fixed::operator>=(const Fixed& other) const { return this->_rawBits >= other._rawBits; }
bool Fixed::operator<=(const Fixed& other) const { return this->_rawBits <= other._rawBits; }
bool Fixed::operator==(const Fixed& other) const { return this->_rawBits == other._rawBits; }
bool Fixed::operator!=(const Fixed& other) const { return this->_rawBits != other._rawBits; }

/* --- 4 Arithmetic Operators --- */

Fixed Fixed::operator+(const Fixed& other) const {
    Fixed res;
    res.setRawBits(this->_rawBits + other._rawBits);
    return res;
}

Fixed Fixed::operator-(const Fixed& other) const {
    Fixed res;
    res.setRawBits(this->_rawBits - other._rawBits);
    return res;
}

// 掛け算: (A * B) / 256
Fixed Fixed::operator*(const Fixed& other) const {
    Fixed res;
    res.setRawBits(((long)this->_rawBits * other._rawBits) >> _fractionalBits);
    return res;
}

// 割り算: (A * 256) / B
Fixed Fixed::operator/(const Fixed& other) const {
    Fixed res;
    res.setRawBits(((long)this->_rawBits << _fractionalBits) / other._rawBits);
    return res;
}

/* --- 4 Increment/Decrement Operators --- */

// 前置 (++a): 最小単位(1)を足して、自分自身を返す
Fixed& Fixed::operator++(void) {
    this->_rawBits++;
    return *this;
}

// 後置 (a++): 増やす前のコピーを返し、自分は増える
Fixed Fixed::operator++(int) {
    Fixed temp(*this);
    this->_rawBits++;
    return temp;
}

Fixed& Fixed::operator--(void) {
    this->_rawBits--;
    return *this;
}

Fixed Fixed::operator--(int) {
    Fixed temp(*this);
    this->_rawBits--;
    return temp;
}

/* --- 4 Static Min/Max Functions --- */

Fixed& Fixed::min(Fixed& a, Fixed& b) {
    return (a < b) ? a : b;
}

const Fixed& Fixed::min(const Fixed& a, const Fixed& b) {
    return (a < b) ? a : b;
}

Fixed& Fixed::max(Fixed& a, Fixed& b) {
    return (a > b) ? a : b;
}

const Fixed& Fixed::max(const Fixed& a, const Fixed& b) {
    return (a > b) ? a : b;
}

/* --- Ostream Operator Overload --- */

std::ostream& operator<<(std::ostream& os, const Fixed& fixed) {
    os << fixed.toFloat();
    return os;
}
