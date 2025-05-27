#include "ft_printf_bonus.h"

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
int print_integer(t_format *fmt, int decimal)
{
	t_render_info info;
	info = compute_signed_render_info(fmt, decimal);
	render_output(fmt, &info);
	free(info.str);
	return (info.total_len);
}


int print_unsigned_decimal(t_format *fmt, unsigned int decimal)
{
	t_render_info info;
	info = compute_unsigned_render_info(fmt, decimal, "0123456789", NULL, 0);
	render_output(fmt, &info);
	free(info.str);
	return (info.total_len);
}