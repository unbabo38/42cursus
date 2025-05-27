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
	info = compute_render_info(fmt, decimal);
	render_integer(fmt, &info);
	free(info.str);
	return (info.total_len);
}

t_render_info compute_render_info(t_format *fmt, int decimal)
{
	t_render_info info;
	int is_negative;
	const char *ten_base = "0123456789";

	is_negative = (decimal < 0);
	info.sign_char = sign_check(fmt, decimal);
	info.sign_len = (info.sign_char != '\0');
	if (is_negative)
		decimal = -decimal;
	info.str = itoa_base(decimal, ten_base);
	info.len = ft_strlen(info.str);
	if (fmt->has_precision && fmt->precision == 0 && decimal == 0)
		info.len = 0;
	info.pad_zero = 0;
	if (fmt->precision > info.len)
		info.pad_zero = fmt->precision - info.len;
	info.total_len = info.sign_len + info.pad_zero + info.len;
	info.pad_space = max(fmt->width - info.total_len, 0);
	info.total_len += info.pad_space;
	return (info);
}

void render_integer(t_format *fmt, t_render_info *info)
{
	if (fmt->flg & FLG_LEFT)
		render_left_aligned_integer(info);
	else
		render_right_aligned_integer(fmt, info);
}

void render_left_aligned_integer(t_render_info *info)
{
	if (info->sign_char)
		write(1, &info->sign_char, 1);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
	while (info->pad_space-- > 0)
		write(1, " ", 1);
}

void render_right_aligned_integer(t_format *fmt, t_render_info *info)
{
	if ((fmt->flg & FLG_ZERO) && !fmt->has_precision)
		render_right_zero_padded(info);
	else
		render_right_space_padded(info);
}

void render_right_zero_padded(t_render_info *info)
{
	if (info->sign_char)
		write(1, &info->sign_char, 1);
	while (info->pad_space-- > 0)
		write(1, "0", 1);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
}

void render_right_space_padded(t_render_info *info)
{
	while (info->pad_space-- > 0)
		write(1, " ", 1);
	if (info->sign_char)
		write(1, &info->sign_char, 1);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
}
