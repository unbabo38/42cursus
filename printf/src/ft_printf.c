/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:49:12 by tmura             #+#    #+#             */
/*   Updated: 2025/05/30 10:52:01 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	int		total_len;

	va_start(ap, s);
	total_len = handle_format_loop(s, &ap);
	va_end(ap);
	if (total_len == ERROR)
		return (ERROR);
	return (total_len);
}

int	handle_format_loop(const char *s, va_list *ap)
{
	int	i;
	int	total_len;

	i = 0;
	total_len = 0;
	while (s[i])
	{
		if (s[i] == '%')
			total_len += handle_percent(s, &i, ap);
		else
		{
			write(1, &s[i], 1);
			total_len++;
			i++;
		}
	}
	return (total_len);
}

int	handle_percent(const char *s, int *i, va_list *ap)
{
	t_format	fmt;
	int			len;

	(*i)++;
	ft_bzero(&fmt, sizeof(t_format));
	scan_line(s, i, ap, &fmt);
	len = print_arg(&fmt, ap);
	return (len);
}

int	print_arg(t_format *fmt, va_list *ap)
{
	if (fmt->specifier == 'c')
		return (print_char(fmt, va_arg(*ap, int)));
	else if (fmt->specifier == 's')
		return (print_string(fmt, va_arg(*ap, char *)));
	else if (fmt->specifier == 'd' || fmt->specifier == 'i')
		return (print_decimal(fmt, va_arg(*ap, int)));
	else if (fmt->specifier == 'u')
		return (print_unsigned_decimal(fmt, va_arg(*ap, unsigned int)));
	else if (fmt->specifier == 'x')
		return (print_hexadecimal_low(fmt, va_arg(*ap, unsigned int)));
	else if (fmt->specifier == 'X')
		return (print_hexadecimal_up(fmt, va_arg(*ap, unsigned int)));
	else if (fmt->specifier == 'p')
		return (print_pointer(fmt, va_arg(*ap, void *)));
	else if (fmt->specifier == '%')
		return (print_percent());
	return (0);
}

/*
int	ft_printf(const char *s, ...)
{
	va_list	ap;
	if (!s)
		return (-1);
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
	*/
/*
#include <limits.h>
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
	//ft_printf("%p", NULL);
	//printf("%d", printf(" %2p ", -1));
	//printf("%d", ft_printf(" %2p ", -1));
	//char *str = "ok";
	//ft_printf(NULL, 1);
	//printf("%p", &str);
	//printf("%s", NULL);
 	printf("%d", printf("%s", "a"));
	printf("%d", ft_printf("%s", "a"));
}

*/
