#include "Span.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

void testBasic() {
    std::cout << "=== Basic test ===" << std::endl;
    Span sp(5);
    sp.addNumber(6);
    sp.addNumber(3);
    sp.addNumber(17);
    sp.addNumber(9);
    sp.addNumber(11);

    std::cout << "shortestSpan: " << sp.shortestSpan() << std::endl; // 2
    std::cout << "longestSpan: " << sp.longestSpan() << std::endl;   // 14

    try {
        sp.addNumber(100);
    } catch (const std::exception &e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
}

void testExceptions() {
    std::cout << "\n=== Exception test ===" << std::endl;
    Span sp(3);
    try {
        sp.shortestSpan();
    } catch (const std::exception &e) {
        std::cout << "Caught (empty): " << e.what() << std::endl;
    }

    sp.addNumber(42);
    try {
        sp.longestSpan();
    } catch (const std::exception &e) {
        std::cout << "Caught (only one): " << e.what() << std::endl;
    }
}

void testLargeScale(unsigned int n) {
    std::cout << "\n=== Large scale test (" << n << " numbers) ===" << std::endl;

    std::srand(static_cast<unsigned int>(std::time(NULL)));

    Span sp(n);
    for (unsigned int i = 0; i < n; ++i) {
        sp.addNumber(std::rand());
    }

    std::cout << "Size: " << sp.getNums().size() << std::endl;
    std::cout << "shortestSpan: " << sp.shortestSpan() << std::endl;
    std::cout << "longestSpan: " << sp.longestSpan() << std::endl;

    try {
        sp.addNumber(1);
    } catch (const std::exception &e) {
        std::cout << "Caught expected exception on full Span: " << e.what() << std::endl;
    }
}

int main() {
    testBasic();
    testExceptions();
    testLargeScale(10000);
    testLargeScale(1000000); // 100万個でも試す

    return 0;
}
