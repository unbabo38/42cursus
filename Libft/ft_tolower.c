#include "libft.h"
#include <ctype.h>


int ft_tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}
/*

#include <stdio.h>
int main(void)
{
    int i;
    int std_result;
    int ft_result;

    printf("Testing ft_toupper:\n\n");

    for (i = -256; i <= 256; i++) // テスト範囲を広く取る
    {
        std_result = tolower(i);
        ft_result = ft_tolower(i);
        printf("Mismatch for input %d: tolower = %d, ft_tolower = %d\n", i, std_result, ft_result);
    }

    printf("Test completed.\n");

    return 0;
}
*/
