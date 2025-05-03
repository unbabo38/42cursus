#include "libft.h"
#include <string.h>
#include <stdio.h>

void *ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest || !src)
		return NULL;
	while (i < n) {
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
/*
int main()
{
	unsigned char buffer1[10] = {1, 2, 3, 4, 5, 0}; // テスト用データ
	unsigned char buffer2[10] = {0};

	ft_memcpy(buffer2, buffer1, 4);

	if (memcmp(buffer1, buffer2, 4) == 0)
	{
		printf("Test passed!\n");
	}
	else
	{
		printf("Test failed!\n");
	}
	return 0;
}
*/
