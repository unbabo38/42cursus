#include "easyfind.hpp"

int main() {
    std::vector<int> n(10, 0);
    n[3] = 42;

    try {
        std::vector<int>::iterator res = easyfind(n, 42);
        std::cout << "found: " << *res << std::endl;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
