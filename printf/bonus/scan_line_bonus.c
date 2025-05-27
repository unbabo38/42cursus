#include "ft_printf_bonus.h"




void scan_line(const char *line, int *i, va_list *ap, t_format *fmt)
{
	parse_flags(line, i, fmt);
	parse_width(line, i, ap, fmt);
	parse_precision(line, i, ap, fmt);
	parse_specifier(line, i, fmt);
}

void parse_flags(const char *line, int *i, t_format *fmt)
{
	while(1)
	{
		if (line[*i] == '-')
			fmt->flg |= FLG_LEFT;
		else if (line[*i] == '0')
			fmt->flg |= FLG_ZERO;
		else if (line[*i] == '+')
			fmt->flg |= FLG_PLUS;
		else if (line[*i] == ' ')
			fmt->flg |= FLG_SPACE;
		else if (line[*i] == '#')
			fmt->flg |= FLG_HASH;
		else
			break ;
		(*i)++;
	}
}

void	parse_width(const char *s, int *i, va_list *ap, t_format *fmt)
{
	if (s[*i] == '*')
	{
		fmt->width = va_arg(*ap, int);
		(*i)++;
	}
	else if (ft_isdigit(s[*i]))
	{
    	fmt->width = ft_atoi(&s[*i]);
    	while (ft_isdigit(s[*i]))
        	(*i)++;
	}
	if (fmt->width < 0)
	{
		fmt->flg |= FLG_LEFT;
		fmt->width = -(fmt->width);
	}
}
void	parse_precision(const char *s, int *i, va_list *ap, t_format *fmt)
{
	if (s[*i] && s[*i] == '.')
	{
		fmt->has_precision = 1;
		(*i)++;
		if (s[*i] && s[*i] == '*')
		{
			fmt->precision = va_arg(*ap, int);
			(*i)++;
		}
		else if (s[*i] && ft_isdigit(s[*i]))
		{
    		fmt->precision = ft_atoi(&s[*i]);
    		while (ft_isdigit(s[*i]))
    	    	(*i)++;
		}
		else
			fmt->precision = 0;
	}
}
void	parse_specifier(const char *line, int *i, t_format *fmt)
{
	if (line[*i] == 'c')
		fmt->specifier = 'c';
	else if (line[*i] == 's')
		fmt->specifier = 's';
	else if (line[*i] == 'p')
		fmt->specifier = 'p';
	else if (line[*i] == 'd')
		fmt->specifier = 'd';
	else if (line[*i] == 'i')
		fmt->specifier = 'i';
	else if (line[*i] == 'u')
		fmt->specifier = 'u';
	else if (line[*i] == 'x')
		fmt->specifier = 'x';
	else if (line[*i] == 'X')
		fmt->specifier = 'X';
	else if (line[*i] == '%')
		fmt->specifier = '%';
	else
		return ;
	(*i)++;
}

int print_arg(t_format *fmt, va_list *ap)
{
	if (fmt->specifier == 'c')
		return print_char(fmt, va_arg(*ap, int));
	else if (fmt->specifier == 's')
		return print_string(fmt, va_arg(*ap, char *));
	else if (fmt->specifier == 'd' || fmt->specifier == 'i')
		return print_integer(fmt, va_arg(*ap, int));
	else if (fmt->specifier == 'u')
		return print_unsigned_decimal(fmt, va_arg(*ap, unsigned int));
	else if (fmt->specifier == 'x')
		return print_hexadecimal_low(fmt, va_arg(*ap, unsigned int));
	else if (fmt->specifier == 'X')
		return print_hexadecimal_up(fmt, va_arg(*ap, unsigned int));
	else if (fmt->specifier == 'p')
		return print_pointer(fmt, va_arg(*ap, void *));
	else if (fmt->specifier == '%')
		return print_percent(fmt);
	return (0);
}
