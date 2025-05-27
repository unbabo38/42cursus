#include "ft_printf_bonus.h"

int	max(int left, int right)
{
	if (left >= right)
		return left;
	else
		return right;
}
int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (2048);
	else
		return (0);
}
int	ft_atoi(const char *str)
{
	long	i;
	long	nbr;
	int		isneg;

	i = 0;
	nbr = 0;
	isneg = 0;
	while (str[i] != '\0' && (str[i] == 32 || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
		i++;
	if (str[i] != '\0' && str[i] == '-')
	{
		isneg = 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0' && ft_isdigit(str[i]))
		nbr = (nbr * 10) + (str[i++] - '0');
	if (isneg == 1)
		return (-nbr);
	return (nbr);
}
int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		i++;

	}
	//printf("%d\n", i);

	return (i);
}

int print_char(t_format *fmt, const char c)
{
	int i;

	i = 0;
	if (fmt->flg & FLG_LEFT)
	{
		write(1, &c, 1);
		while (i < fmt->width - 1)
		{
			write(1, " ", 1);
			i++;
		}
	}
	else
	{
		while (i < fmt->width - 1)
		{
			write(1, " ", 1);
			i++;
		}
		write(1, &c, 1);
	}
	return (i + 1);
}
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/*
int print_string(t_format *fmt, const char *string)
{
	int len = 0;
	int actual_padding = 0;
	int padding = 0;
	const char *str;
	if (!string)
		str = "(null)";
	else
		str = string;
	len = ft_strlen(str);

	if (fmt->has_precision && fmt->precision < len)
		len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0)
		len = 0;
	if (!string && fmt->has_precision && fmt->precision <=5)
		len = 0;
	actual_padding = max(fmt->width - len, 0);

	padding = actual_padding;
	if (fmt->flg & FLG_LEFT)
	{
		write(1, str, len);
		while (padding-- > 0)
			write(1, " ", 1);
	}
	else
	{
		while (padding-- > 0)
			write(1, " ", 1);
		write(1, str, len);
	}
	return (len + actual_padding);
}
	*/
int print_percent(t_format *fmt)
{
	(void)fmt;
	char percent = '%';
	write(1, &percent, 1);
	return (1);
}
char sign_check(t_format *fmt, int decimal)
{
	if (decimal < 0)
		return ('-');
	if (fmt->flg & FLG_PLUS)
		return ('+');
	if (fmt->flg & FLG_SPACE)
		return (' ');
	else
		return ('\0');
}
/*
int print_decimal(t_format *fmt, int decimal)
{
	const char *ten_base = "0123456789";
	int is_negative;
	char sign_char = '\0';
	is_negative = (decimal < 0);
	sign_char = sign_check(fmt, decimal);
	int sign_len = (sign_char != '\0');
	if (is_negative)
		decimal = -decimal;
	//print("decimal:%d\n", decimal);
	char *char_decimal = itoa_base(decimal, ten_base);

	int	len = ft_strlen(char_decimal);


	int precision_padding;
	precision_padding = 0;
	//if (fmt->has_precision && fmt->precision < len)
	//	len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0 && decimal == 0)
		len = 0;
	if (fmt->precision > len)
		precision_padding = fmt->precision - len;
	int total_len = sign_len + precision_padding + len;
	int padding = max(fmt->width - total_len, 0);
	total_len += padding;
	if (fmt->flg & FLG_LEFT)
	{
		if (sign_char)
			write(1, &sign_char, 1);
		while (precision_padding-- > 0)
    		write(1, "0", 1);
		if (len > 0)
			write(1, char_decimal, len);
		while (padding-- > 0)
    		write(1, " ", 1);
	}
	else
	{
		if (fmt->flg & FLG_ZERO && !fmt->has_precision)
		{
			if (sign_char)
				write(1, &sign_char, 1);
			while (padding-- > 0)
    			write(1, "0", 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, char_decimal, len);
		}
		else
		{
			//printf("%s", char_decimal);
			//printf("%d", len);
			while (padding-- > 0)
    			write(1, " ", 1);
			if (sign_char)
				write(1, &sign_char, 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, char_decimal, len);
		}
	}
	free(char_decimal);
	return (total_len);
}
	*/
/*
int print_integer(t_format *fmt, int decimal)
{
	int is_negative;
	char sign_char = '\0';
	is_negative = (decimal < 0);
	sign_char = sign_check(fmt, decimal);
	int sign_len = (sign_char != '\0');
	if (is_negative)
		decimal = -decimal;

	char *char_decimal = itoa_base(decimal, ten_base);
	int	len = ft_strlen(char_decimal);

	int precision_padding;
	precision_padding = 0;
	//if (fmt->has_precision && fmt->precision < len)
	//	len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0 && decimal == 0)
		len = 0;
	if (fmt->precision > len)
		precision_padding = fmt->precision - len;
	int total_len = sign_len + precision_padding + len;
	int padding = max(fmt->width - total_len, 0);
	total_len += padding;

	if (fmt->flg & FLG_LEFT)
	{
		if (sign_char)
			write(1, &sign_char, 1);
		while (precision_padding-- > 0)
    		write(1, "0", 1);
		if (len > 0)
			write(1, char_decimal, len);
		while (padding-- > 0)
    		write(1, " ", 1);
	}
	else
	{
		if (fmt->flg & FLG_ZERO && !fmt->has_precision)
		{
			if (sign_char)
				write(1, &sign_char, 1);
			while (padding-- > 0)
    			write(1, "0", 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, char_decimal, len);
		}
		else
		{
			while (padding-- > 0)
    			write(1, " ", 1);
			if (sign_char)
				write(1, &sign_char, 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, char_decimal, len);
		}
	}
	free(char_decimal);
	return (total_len);
}
*/
/*
int print_unsigned_decimal(t_format *fmt, unsigned int decimal)
{
	const char *ten_base = "0123456789";
	char *char_decimal;
	char_decimal = itoa_base(decimal, ten_base);
	int	len = ft_strlen(char_decimal);


	int precision_padding;
	precision_padding = 0;
	//if (fmt->has_precision && fmt->precision < len)
	//	len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0 && decimal == 0)
		len = 0;
	if (fmt->precision > len)
		precision_padding = fmt->precision - len;
	int total_len = precision_padding + len;
	int padding = max(fmt->width - total_len, 0);
	total_len += padding;
	if (fmt->flg & FLG_LEFT)
	{
		while (precision_padding-- > 0)
    		write(1, "0", 1);
		if (len > 0)
			write(1, char_decimal, len);
		while (padding-- > 0)
    		write(1, " ", 1);
	}
	else
	{
		if (fmt->flg & FLG_ZERO && !fmt->has_precision)
		{
			while (padding-- > 0)
    			write(1, "0", 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, char_decimal, len);
		}
		else
		{
			while (padding-- > 0)
    			write(1, " ", 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, char_decimal, len);
		}
	}
	free(char_decimal);
	return (total_len);
}
*/
char *itoa_base_unsigned(unsigned long long n, const char *base)
{
	unsigned long long tmp = n;
	int digit;
	unsigned long long len;
	char *num;

	len = ft_strlen(base);
	digit = 0;
	if (n == 0)
		digit++;
	while (tmp > 0)
	{
		digit++;
		tmp /= len;
	}
	num = malloc(digit + 1);
	if (!num)
		return (NULL);
	if (n == 0)
		num[0] = '0';
	num[digit] = '\0';
	while (digit-- > 0)
	{
		num[digit] = (base[n % len]);
		n /= len;
	}
	return (num);
}

char *itoa_base(long long n, const char *base)
{
	long long tmp;
	if (n < 0)
		n = (long long)(-(n + 1)) + 1;
	else
		n = (long long)n;
	tmp = n;
	int digit;
	unsigned long long len;
	char *num;

	len = ft_strlen(base);
	digit = 0;
	if (n == 0)
		digit++;
	while (tmp > 0)
	{
		digit++;
		tmp /= len;
	}
	num = malloc(digit + 1);
	if (!num)
		return (NULL);
	if (n == 0)
		num[0] = '0';
	num[digit] = '\0';
	while (digit-- > 0)
	{
		num[digit] = (base[n % len]);
		n /= len;
	}
	return (num);
}
/*
int print_hexadecimal_low(t_format *fmt, unsigned int hex_decimal_low)
{
	char *num = itoa_base(hex_decimal_low, low_base);
	int	len = ft_strlen(num);
	int precision_padding;
	precision_padding = 0;
	int prefix_len = 0;
	if ((fmt->flg & FLG_HASH) && hex_decimal_low != 0)
		prefix_len = 2;
	//if (fmt->has_precision && fmt->precision < len)
	//	len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0 && hex_decimal_low == 0)
		len = 0;
	if (fmt->precision > len)
		precision_padding = fmt->precision - len;
	int total_len = prefix_len + precision_padding + len;
	int padding = max(fmt->width - total_len, 0);
	total_len += padding;
	if (fmt->flg & FLG_LEFT)
	{
		if (prefix_len)
			write(1, "0x", 2);
		while (precision_padding-- > 0)
    		write(1, "0", 1);
		if (len > 0)
			write(1, num, len);
		while (padding-- > 0)
    		write(1, " ", 1);
	}
	else
	{
		if (fmt->flg & FLG_ZERO && !fmt->has_precision)
		{
			if (prefix_len)
				write(1, "0x", 2);
			while (padding-- > 0)
    			write(1, "0", 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, num, len);
		}
		else
		{
			while (padding-- > 0)
    			write(1, " ", 1);
			if (prefix_len)
				write(1, "0x", 2);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, num, len);
		}
	}
	free(num);
	return (total_len);
}
	*/
	/*
int print_hexadecimal_up(t_format *fmt, unsigned int hex_decimal_up)
{
	const char *up_base = "0123456789ABCDEF";

	char *num = itoa_base(hex_decimal_up, up_base);
	int	len = ft_strlen(num);
	int precision_padding;
	precision_padding = 0;
	int prefix_len = 0;
	if ((fmt->flg & FLG_HASH) && hex_decimal_up != 0)
		prefix_len = 2;
	//if (fmt->has_precision && fmt->precision < len)
	//	len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0 && hex_decimal_up == 0)
		len = 0;
	if (fmt->precision > len)
		precision_padding = fmt->precision - len;
	int total_len = prefix_len + precision_padding + len;
	int padding = max(fmt->width - total_len, 0);
	total_len += padding;
	if (fmt->flg & FLG_LEFT)
	{
		if (prefix_len)
			write(1, "0X", 2);
		while (precision_padding-- > 0)
    		write(1, "0", 1);
		if (len > 0)
			write(1, num, len);
		while (padding-- > 0)
    		write(1, " ", 1);
	}
	else
	{
		if (fmt->flg & FLG_ZERO && !fmt->has_precision)
		{
			if (prefix_len)
				write(1, "0X", 2);
			while (padding-- > 0)
    			write(1, "0", 1);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, num, len);
		}
		else
		{
			while (padding-- > 0)
    			write(1, " ", 1);
			if (prefix_len)
				write(1, "0X", 2);
			while (precision_padding-- > 0)
    			write(1, "0", 1);
			if (len > 0)
				write(1, num, len);
		}
	}
	free(num);
	return (total_len);
}
	*/
	/*
int print_pointer(t_format *fmt, void *ptr)
{
	const char *low_base = "0123456789abcdef";
	unsigned long long ptr_num = (unsigned long long)ptr;
	if (ptr_num == 0)
	{
		print_string(fmt, "(nil)");
		return (5);
	}
	char *num = itoa_base_unsigned(ptr_num, low_base);
	int	len = ft_strlen(num);
	int prefix_len = 2;
	//if (fmt->has_precision && fmt->precision < len)
	//	len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0 && ptr_num == 0)
		len = 0;
	int total_len = prefix_len + len;
	int padding = max(fmt->width - total_len, 0);
	total_len += padding;
	if (fmt->flg & FLG_LEFT)
	{
		if (prefix_len)
			write(1, "0x", 2);
		if (len > 0)
			write(1, num, len);
		while (padding-- > 0)
    		write(1, " ", 1);
	}
	else
	{
		while (padding-- > 0)
    		write(1, " ", 1);
		if (prefix_len)
			write(1, "0x", 2);
		if (len > 0)
			write(1, num, len);
	}
	free(num);
	return (total_len);
}
*/
