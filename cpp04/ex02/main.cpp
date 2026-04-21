#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"


int main()
{
    Animal* animal = new Cat();
	animal->makeSound(); // "Meow!" と出力される

	delete animal;
}
