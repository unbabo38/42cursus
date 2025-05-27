/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_hexadecimal_low_bonus_utils.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:04:29 by tmura             #+#    #+#             */
/*   Updated: 2025/05/27 14:04:31 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	render_hexadecimal_low(t_format *fmt, t_render_info *info)
{
	if (fmt->flg & FLG_LEFT)
		render_left_aligned_hex(info);
	else
		render_right_aligned_hex(fmt, info);
}

void	render_left_aligned_hex(t_render_info *info)
{
	if (info->prefix_len)
		write(1, "0x", 2);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
	while (info->pad_space-- > 0)
		write(1, " ", 1);
}

void	render_right_aligned_hex(t_format *fmt, t_render_info *info)
{
	if ((fmt->flg & FLG_ZERO) && !fmt->has_precision)
		render_right_zero_padded_hex(info);
	else
		render_right_space_padded_hex(info);
}

void	render_right_zero_padded_hex(t_render_info *info)
{
	if (info->prefix_len)
		write(1, "0x", 2);
	while (info->pad_space-- > 0)
		write(1, "0", 1);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
}

void	render_right_space_padded_hex(t_render_info *info)
{
	while (info->pad_space-- > 0)
		write(1, " ", 1);
	if (info->prefix_len)
		write(1, "0x", 2);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
}
