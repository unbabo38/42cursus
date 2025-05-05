/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 21:17:54 by tmura             #+#    #+#             */
/*   Updated: 2025/05/05 18:36:27 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
/*
#include <stdio.h>
#include <assert.h>
int	main(void)
{
	size_t count = 10;
	size_t size = sizeof(int);

	int *arr = (int *)ft_calloc(count, size);
	assert(arr != NULL);

	for (size_t i = 0; i < count; i++)
	{
		if (arr[i] != 0)
		{
			printf("Test failed: arr[%zu] != 0 (value = %d)\n", i, arr[i]);
			free(arr);
			return (1);
		}
	}

	printf("Test passed: all values are 0.\n");
	free(arr);
	return (0);
}
*/
