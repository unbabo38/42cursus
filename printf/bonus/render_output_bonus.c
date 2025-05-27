#include "ft_printf_bonus.h"

void render_output(t_format *fmt, t_render_info *info)
{
	if (fmt->flg & FLG_LEFT)
		render_left(info);
	else if ((fmt->flg & FLG_ZERO) && !fmt->has_precision)
		render_right_zero(info);
	else
		render_right_space(info);
}

void render_left(t_render_info *info)
{
	if (info->sign_char)
		write(1, &info->sign_char, 1);
	if (info->prefix_str)
		write(1, info->prefix_str, info->prefix_len);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
	while (info->pad_space-- > 0)
		write(1, " ", 1);
}


void render_right_zero(t_render_info *info)
{
	if (info->sign_char)
		write(1, &info->sign_char, 1);
	if (info->prefix_str)
		write(1, info->prefix_str, info->prefix_len);
	while (info->pad_space-- > 0)
		write(1, "0", 1);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
}

void render_right_space(t_render_info *info)
{
	while (info->pad_space-- > 0)
		write(1, " ", 1);
	if (info->sign_char)
		write(1, &info->sign_char, 1);
	if (info->prefix_str)
		write(1, info->prefix_str, info->prefix_len);
	while (info->pad_zero-- > 0)
		write(1, "0", 1);
	if (info->len > 0)
		write(1, info->str, info->len);
}
