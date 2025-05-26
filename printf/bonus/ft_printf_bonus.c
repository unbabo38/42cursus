#include "ft_printf.h"
#include <stdio.h>
int ft_printf(const char *s, ...)
{
	va_list ap;
	va_start(ap, s);
	int i = 0;
	int total_len = 0;

	while (s[i])
	{
		if (s[i] == '%')
		{
			i++;
			t_format fmt = {0};
			scan_line(s, &i, &ap, &fmt);
			total_len += print_arg(&fmt, &ap);
		}
		else
		{
			write(1, &s[i], 1);
			total_len++;
			i++;
		}
	}
	va_end(ap);
	return (total_len);
}
/*
int main()
{
	int k = 10;
	//void *t = k;
	ft_printf(" %s \n", "-");
	ft_printf(" %s %s \n", "", "-");
	ft_printf(" %s %s \n", " - ", "");
	ft_printf(" %s %s %s %s %s \n", " - ", "", "4", "", "2 ");
	ft_printf(" NULL %s NULL \n", NULL);
	//ft_printf("%s", "adf");
	printf(" %s \n", "-");
	printf(" %s %s \n", "", "-");
	printf(" %s %s \n", " - ", "");
	printf(" %s %s %s %s %s \n", " - ", "", "4", "", "2 ");
	printf(" NULL %s NULL \n", NULL);
}
*/
