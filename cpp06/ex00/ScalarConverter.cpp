#include "ScalarConverter.hpp"
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter &other) { (void)other; }
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other) { (void)other; return *this; }
ScalarConverter::~ScalarConverter() {}

static bool is_printable(char c) {
    return (c >= 32 && c <= 126);
}

static bool is_pseudo_literal(const std::string &str) {
    return (str == "nan" || str == "nanf" || str == "inf" || str == "+inf" ||
            str == "inff" || str == "+inff" || str == "-inf" || str == "-inff");
}

static void handle_pseudo(const std::string &str) {
    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;
    if (str == "nan" || str == "nanf") {
        std::cout << "float: nanf" << std::endl;
        std::cout << "double: nan" << std::endl;
    } else if (str == "-inf" || str == "-inff") {
        std::cout << "float: -inff" << std::endl;
        std::cout << "double: -inf" << std::endl;
    } else {
        std::cout << "float: inff" << std::endl;
        std::cout << "double: inf" << std::endl;
    }
}

void ScalarConverter::convert(const std::string &str) {
    if (is_pseudo_literal(str)) {
        handle_pseudo(str);
        return;
    }

    if (str.length() == 1 && !std::isdigit(str[0])) {
        char c = str[0];
        std::cout << "char: '" << c << "'" << std::endl;
        std::cout << "int: " << static_cast<int>(c) << std::endl;
        std::cout << "float: " << std::fixed << std::setprecision(1) << static_cast<float>(c) << "f" << std::endl;
        std::cout << "double: " << std::fixed << std::setprecision(1) << static_cast<double>(c) << std::endl;
        return;
    }

    char *end;
    double val = std::strtod(str.c_str(), &end);

    if (*end != '\0' && !(*end == 'f' && *(end + 1) == '\0')) {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible" << std::endl;
        std::cout << "double: impossible" << std::endl;
        return;
    }

    if (val < 0 || val > 127 || std::isnan(val)) {
        std::cout << "char: impossible" << std::endl;
    } else {
        char c = static_cast<char>(val);
        if (is_printable(c)) {
            std::cout << "char: '" << c << "'" << std::endl;
        } else {
            std::cout << "char: Non displayable" << std::endl;
        }
    }

    if (val < INT_MIN || val > INT_MAX || std::isnan(val)) {
        std::cout << "int: impossible" << std::endl;
    } else {
        std::cout << "int: " << static_cast<int>(val) << std::endl;
    }

    std::cout << "float: " << std::fixed << std::setprecision(1) << static_cast<float>(val) << "f" << std::endl;
    std::cout << "double: " << std::fixed << std::setprecision(1) << val << std::endl;
}
