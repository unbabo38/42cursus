#include <iostream>
#include <Phonebook.hpp>

PhoneBook::PhoneBook()
{
	index = 0;
	is_full = false;
}
PhoneBook::~PhoneBook(){}

void	PhoneBook::add()
{
	this->_contact[index].set_first_name(get_input("first_name"));
	this->_contact[index].set_last_name(get_input("last_name"));
	this->_contact[index].set_nick_name(get_input("nick_name"));
	this->_contact[index].set_phone_number(get_input("phone_number"));
	this->_contact[index].set_secret(get_input("secret"));
	this->index++;
	std::cout << "\n";
	if (index == 8)
	{
		this->is_full = true;
		this->index = 0;
	}
}

void	PhoneBook::search()
{
	
}
