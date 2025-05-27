#include "ft_printf.h"
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
#include <limits.h>
/*
int main()
{
	//int myfunc = ft_printf(" %s ", "-");
	//printf("%d", myfunc);
	//ft_printf(" %s %s \n", "", "-");
	//ft_printf(" %s %s \n", " - ", "");
	//ft_printf(" %s %s %s %s %s \n", " - ", "", "4", "", "2 ");
	//ft_printf(" NULL %s NULL \n", NULL);
	//int original = printf(" %s ", "-");
	//printf("%d", original);

	//ft_printf(" %p ", LONG_MIN);
	//printf(" %p %p ", LONG_MIN, LONG_MAX);
	//ft_printf(" %d", );
	//ft_printf(" %d ", INT_MIN);
	//ft_printf(" %p %p ", 0, 0);
	//ft_printf("%5%");
	//printf("%5%");
	//ft_printf("%7.5s", "bombastic");
	//ft_printf("%.03s", NULL);
	//printf("%.03s", NULL);
	//printf("%.03d", 42);
	//printf("%.3i", 13862);
	//ft_printf("%.3i", 13862);
	//ft_printf("%3.1s", NULL);
	//ft_printf("percent 2 %12%");
	ft_printf("%p", NULL);
}
*/
