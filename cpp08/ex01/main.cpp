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

	Span same(5);
	same.addNumber(1);
	same.addNumber(1);
	same.addNumber(1);
	same.addNumber(1);
	same.addNumber(1);

	Span bigName(2);
	bigName.addNumber(2147483647);
	bigName.addNumber(-2147483648);

	std::cout << "---3, 6, 9, 11, 17のテスト---" << std::endl;
    std::cout << "shortestSpan: " << sp.shortestSpan() << std::endl;
    std::cout << "longestSpan: " << sp.longestSpan() << std::endl;

    std::cout << "---全部1のテスト---" << std::endl;
    std::cout << "same number shortestSpan: " << same.shortestSpan() << std::endl;
    std::cout << "same number longestSpan: " << same.longestSpan() << std::endl;

    std::cout << "---INTMAX,INTMINのテスト---" << std::endl;
    std::cout << "int max min shortestSpan: " << bigName.shortestSpan() << std::endl;
    std::cout << "int max min longestSpan: " << bigName.longestSpan() << std::endl;

    try {
        sp.addNumber(100);
    } catch (const std::exception &e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
}

void testExceptions() {
    std::cout << "\n---例外のテスト(空データ、データ数１)---" << std::endl;
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
    std::cout << "\n大きなスケールのテスト (" << n << " numbers) ---" << std::endl;

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
    testLargeScale(1000000);

    return 0;
}
