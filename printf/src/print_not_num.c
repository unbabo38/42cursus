/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_not_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:49:26 by tmura             #+#    #+#             */
/*   Updated: 2025/05/30 10:49:14 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	print_char(t_format *fmt, const char c)
{
	int	i;

	i = 0;
	if (fmt->flg & FLG_LEFT)
	{
		if (safe_write(1, &c, 1) == ERROR)
			return (ERROR);
		while (i < fmt->width - 1)
		{
			if (safe_write(1, " ", 1) == ERROR)
				return (ERROR);
			i++;
		}
	}
	else
	{
		while (i < fmt->width - 1)
		{
			if (safe_write(1, " ", 1) == ERROR)
				return (ERROR);
			i++;
		}
		if (safe_write(1, &c, 1) == ERROR)
			return (ERROR);
	}
	return (i + 1);
}

int write_string(t_format *fmt, t_info *info)
{
	if (fmt->flg & FLG_LEFT)
	{
		if (write_left(info) == ERROR)
			return (ERROR);
	}
	else
	{
		if (write_right_not_zero(info) == ERROR)
			return (ERROR);
	}
	return (OK);
}

int	print_string(t_format *fmt, char *string)
{
	int			actual_padding;
	t_info		info;

	actual_padding = 0;
	ft_bzero(&info, sizeof(t_info));
	if (!string)
		info.char_decimal = "(null)";
	else
		info.char_decimal = string;
	info.len = ft_strlen(info.char_decimal);
	if (fmt->has_precision && fmt->precision < info.len)
		info.len = fmt->precision;
	if (fmt->has_precision && fmt->precision == 0)
		info.len = 0;
	if (!string && fmt->has_precision && fmt->precision <= 5)
		info.len = 0;
	actual_padding = max(fmt->width - info.len, 0);
	info.padding = actual_padding;
	if (write_string(fmt, &info) == ERROR)
		return (ERROR);
	return (info.len + actual_padding);
}

int	print_percent(void)
{
	char	percent;

	percent = '%';
	if (safe_write(1, &percent, 1) == ERROR)
		return (ERROR);
	return (1);
}
