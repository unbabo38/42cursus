#include "Zombie.hpp"

int	main() {
	Zombie *zombie1;

	try {
		zombie1 = newZombie("zombie1");
		zombie1->announce();
		delete zombie1;
	} catch (const std::bad_alloc& e) {
		delete zombie1;
		std::cerr << "Error: Memory allocation failed: " << e.what() << std::endl;
		return 1;
	}

	randomChump("zombie2");
}
