/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 21:16:50 by tmura             #+#    #+#             */
/*   Updated: 2025/05/04 21:17:25 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdio.h>

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
}
/*
int main() {
	unsigned char buffer1[10];
	unsigned char buffer2[10];

	bzero(buffer1, 4);
	ft_bzero(buffer2, 4);
	if (memcmp(buffer1, buffer2, 4) == 0)
	{
		printf("%d", 1);
		return 0;
	}
	return 0;
}
*/
