#include "libft.h"
#include <string.h>
#include <stdio.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dsize;

	dsize = 0;
	while (dst[dsize] != '\0' && dsize < size)
		dsize++;
	i = dsize;
	while (src[dsize - i] && dsize + 1 < size)
	{
		dst[dsize] = src[dsize - i];
		dsize++;
	}
	if (i < size)
		dst[dsize] = '\0';
	return (i + ft_strlen(src));
}
/*
int main(void)
{
    char buffer1[20] = "Hello";
    char buffer2[20] = "Hello";
    const char *src = " World";

    printf("=== Test 1: Normal append ===\n");
    size_t ret_std = strlcat(buffer1, src, sizeof(buffer1));
    size_t ret_ft  = ft_strlcat(buffer2, src, sizeof(buffer2));
    printf("strlcat    : %s (return: %zu)\n", buffer1, ret_std);
    printf("ft_strlcat : %s (return: %zu)\n\n", buffer2, ret_ft);

    char buffer3[11] = "Hello";
    char buffer4[11] = "Hello";

    printf("=== Test 2: Exact fit (dstsize = strlen(dst) + strlen(src) + 1) ===\n");
    ret_std = strlcat(buffer3, src, sizeof(buffer3));
    ret_ft  = ft_strlcat(buffer4, src, sizeof(buffer4));
    printf("strlcat    : %s (return: %zu)\n", buffer3, ret_std);
    printf("ft_strlcat : %s (return: %zu)\n\n", buffer4, ret_ft);

    char buffer5[10] = "Hello";
    char buffer6[10] = "Hello";

    printf("=== Test 3: Truncation (dstsize too small) ===\n");
    ret_std = strlcat(buffer5, src, 8);  // small buffer size
    ret_ft  = ft_strlcat(buffer6, src, 8);
    printf("strlcat    : %s (return: %zu)\n", buffer5, ret_std);
    printf("ft_strlcat : %s (return: %zu)\n\n", buffer6, ret_ft);

    char buffer7[5] = "Hi";
    char buffer8[5] = "Hi";

    printf("=== Test 4: Very small buffer ===\n");
    ret_std = strlcat(NULL, NULL, 0);
    ret_ft  = ft_strlcat(NULL, NULL, 0);
    printf("strlcat    : %s (return: %zu)\n", buffer7, ret_std);
    printf("ft_strlcat : %s (return: %zu)\n\n", buffer8, ret_ft);

    return 0;
}
*/
