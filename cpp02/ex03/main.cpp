#include <iostream>
#include "Point.hpp"

int main() {
    Point const a(0, 0);
    Point const b(10, 0);
    Point const c(0, 10);

    Point const in(2, 2);
    Point const out(15, 15);
    Point const edge(5, 0);

    std::cout << "Inside (2,2): " << (bsp(a, b, c, in) ? "True" : "False") << std::endl;
    std::cout << "Outside (15,15): " << (bsp(a, b, c, out) ? "True" : "False") << std::endl;
    std::cout << "On edge (5,0): " << (bsp(a, b, c, edge) ? "True" : "False") << std::endl;

    return 0;
}
