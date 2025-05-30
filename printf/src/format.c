/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:49:06 by tmura             #+#    #+#             */
/*   Updated: 2025/05/30 10:49:04 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	max(int left, int right)
{
	if (left >= right)
		return (left);
	else
		return (right);
}

char	sign_check(t_format *fmt, int decimal)
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

void	format_sign(t_info *info, t_format *fmt, int decimal)
{
	const char	*ten_base;

	ten_base = "0123456789";
	info->sign_char = '\0';
	info->sign_char = sign_check(fmt, decimal);
	info->sign_len = (info->sign_char != '\0');
	if (decimal < 0)
		decimal = -decimal;
	info->char_decimal = itoa_base(decimal, ten_base);
	info->len = ft_strlen(info->char_decimal);
	if (fmt->has_precision && fmt->precision == 0 && decimal == 0)
		info->len = 0;
	if (fmt->precision > info->len)
		info->precision_padding = fmt->precision - info->len;
	info->total_len = info->sign_len + info->precision_padding + info->len;
	info->padding = max(fmt->width - info->total_len, 0);
	info->total_len += info->padding;
}

int	format_unsign(t_info *info, t_format *fmt,
						unsigned long long decimal, char *base)
{
	if (info->prefix_flg)
		info->has_prefix = 2;
	info->char_decimal = itoa_base_unsigned(decimal, base);
	if (!info->char_decimal)
		return (ERROR);
	info->len = ft_strlen(info->char_decimal);
	if (fmt->has_precision && fmt->precision == 0 && decimal == 0)
		info->len = 0;
	if (fmt->precision > info->len)
		info->precision_padding = fmt->precision - info->len;
	info->total_len = info->precision_padding + info->len + info->has_prefix;
	info->padding = max(fmt->width - info->total_len, 0);
	info->total_len += info->padding;
	return (OK);
}

void	format_string(t_info *info, t_format *fmt, char *string)
{
	if (!string)
		info->char_decimal = "(null)";
	else
		info->char_decimal = string;
	info->len = ft_strlen(info->char_decimal);
	if (fmt->has_precision && fmt->precision < info->len)
		info->len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0)
		info->len = 0;
	if (!string && fmt->has_precision && fmt->precision <= 5)
		info->len = 0;
	info->padding = max(fmt->width - info->len, 0);
}
