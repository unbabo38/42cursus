#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"


int main()
{
	// Animal animal1;
	// Dog dog;
	// Cat cat;
	// animal1.makeSound();
	// dog.makeSound();
	// cat.makeSound();

	// Animal* polyDog = new Dog();
	// polyDog->makeSound();

	const Animal* meta = new Animal();
	const Animal* j = new Dog();
	const Animal* i = new Cat();
	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;
	i->makeSound(); //will output the cat sound!
	j->makeSound();
	meta->makeSound();

	delete i;
	delete j;
	delete meta;
}
