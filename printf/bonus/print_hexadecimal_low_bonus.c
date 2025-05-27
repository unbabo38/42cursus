/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexadecimal_low_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:49:42 by tmura             #+#    #+#             */
/*   Updated: 2025/05/27 14:04:15 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"
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

t_render_info	compute_hex_render_info(t_format *fmt, unsigned int value)
{
	t_render_info	info;
	const char		*low_base;

	low_base = "0123456789abcdef";
	info.sign_char = '\0';
	info.sign_len = 0;
	info.str = itoa_base(value, low_base);
	info.len = ft_strlen(info.str);
	if (fmt->has_precision && fmt->precision == 0 && value == 0)
		info.len = 0;
	info.pad_zero = 0;
	if (fmt->precision > info.len)
		info.pad_zero = fmt->precision - info.len;
	info.prefix_len = 0;
	if ((fmt->flg & FLG_HASH) && value != 0)
		info.prefix_len = 2;
	info.total_len = info.prefix_len + info.pad_zero + info.len;
	info.pad_space = max(fmt->width - info.total_len, 0);
	info.total_len += info.pad_space;
	return (info);
}

int	print_hexadecimal_low(t_format *fmt, unsigned int hex_decimal_low)
{
	t_render_info	info;

	info = compute_hex_render_info(fmt, hex_decimal_low);
	render_hexadecimal_low(fmt, &info);
	free(info.str);
	return (info.total_len);
}
