#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void	*ft_calloc(size_t count, size_t size)
{
	void *ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count);
	return (ptr);
}
/*
int	main(void)
{
	size_t count = 10;
	size_t size = sizeof(int);

	int *arr = (int *)ft_calloc(count, size);
	assert(arr != NULL);

	// すべての要素が 0 に初期化されていることを確認
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
