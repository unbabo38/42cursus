#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>
#include <cmath>

class Fixed {
private:
    int _rawBits;
    static const int _fractionalBits = 8;

public:
    // --- Orthodox Canonical Form ---
    Fixed();
    Fixed(const int n);
    Fixed(const float f);
    Fixed(const Fixed& other);
    Fixed& operator=(const Fixed& other);
    ~Fixed();

    // --- Accessors ---
    int getRawBits(void) const;
    void setRawBits(int const raw);

    // --- Conversion ---
    float toFloat(void) const;
    int toInt(void) const;

    // --- 6 Comparison Operators ---
    bool operator>(const Fixed& other) const;
    bool operator<(const Fixed& other) const;
    bool operator>=(const Fixed& other) const;
    bool operator<<= (const Fixed& other) const; // すみません、タイポ防止: <= です
    bool operator<=(const Fixed& other) const;
    bool operator==(const Fixed& other) const;
    bool operator!=(const Fixed& other) const;

    // --- 4 Arithmetic Operators ---
    Fixed operator+(const Fixed& other) const;
    Fixed operator-(const Fixed& other) const;
    Fixed operator*(const Fixed& other) const;
    Fixed operator/(const Fixed& other) const;

    // --- 4 Increment/Decrement Operators ---
    Fixed& operator++(void);    // 前置 (++a)
    Fixed  operator++(int);     // 後置 (a++)
    Fixed& operator--(void);    // 前置 (--a)
    Fixed  operator--(int);     // 後置 (a--)

    // --- 4 Static Min/Max Functions ---
    static Fixed& min(Fixed& a, Fixed& b);
    static const Fixed& min(const Fixed& a, const Fixed& b);
    static Fixed& max(Fixed& a, Fixed& b);
    static const Fixed& max(const Fixed& a, const Fixed& b);
};

// --- Ostream Overload ---
std::ostream& operator<<(std::ostream& os, const Fixed& fixed);

#endif
