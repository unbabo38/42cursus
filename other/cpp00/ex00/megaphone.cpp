/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:18:52 by tmura             #+#    #+#             */
/*   Updated: 2026/03/11 16:18:52 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	main(int argc, char **argv)
{
	if (argc <= 1)
	{
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
		return (0);
	}

	int i = 1;
	while(i < argc)
	{
		int	j = 0;
		while(argv[i][j] != '\0')
		{
			std::cout << (char)std::toupper(argv[i][j]);
			j++;
		}
		i++;
	}
	std::cout << std::endl;
	return (0);
}
