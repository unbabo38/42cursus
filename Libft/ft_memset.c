#include "libft.h"
#include <string.h>
#include <stdio.h>

void *ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;

	if (!s)
	{
		return (NULL);
	}

	while (i < n)
	{

		((unsigned char*)s)[i] = (unsigned char) c;
		i++;
	}
	return (s);
}
/*
int main() {
	unsigned char buffer1[10];
    unsigned char buffer2[10];

    // bufferを初期化
    memset(buffer1, 'A', sizeof(buffer1));
    memset(buffer2, 'A', sizeof(buffer2));

    // テスト①：普通に0埋め
    ft_memset(buffer1, 3, 5);
    memset(buffer2, 3, 5);


    if (memcmp(buffer1, buffer2, 10) == 0)
	{
        printf("Test 1 passed!\n");
	}
	else
    {
	  	printf("Test 1 failed!\n");
	}
	return (0);
}
*/
