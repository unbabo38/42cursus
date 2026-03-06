#ifndef PHONEBOOK_HPP

# define PHONEBOOK_HPP

# include "Contact.hpp"


class	Phonebook
{
	private:
		Contact _contacts[8];
		int		_index;
	public:
		Phonebook(void);
		~Phonebook(void);
		void	add(void);
		void	search(void);
		void	print(Contact contact);
		Contact get_contact(int	index);
};

#endif
