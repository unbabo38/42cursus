#include "Character.hpp"
#include "Cure.hpp"
#include "Ice.hpp"
#include "MateriaSource.hpp"

int main() {
  IMateriaSource *src = new MateriaSource();
  src->learnMateria(new Ice());
  src->learnMateria(new Cure());

  ICharacter *me = new Character("me");
  AMateria *tmp;
  // tmp = src->createMateria("ice");
  // me->equip(tmp);
  tmp = src->createMateria("cure");
  for (int i = 0; i < 1011; i++)
	  me->equip(tmp);

  ICharacter *bob = new Character("bob");
  me->use(0, *bob);
  me->use(1, *bob);

  delete bob;
  delete me;
  delete src;

  // system("leaks interface"); // Mac環境ならこれを入れてリーク確認！
  return 0;
}
