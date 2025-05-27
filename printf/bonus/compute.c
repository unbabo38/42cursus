#include "ft_printf_bonus.h"

t_render_info	compute_unsigned_render_info(
	t_format *fmt,
	unsigned long long val,
	const char *base,
	const char *prefix,
	int use_prefix_when_zero
)
{
	t_render_info	info;

	info.sign_char = '\0';
	info.sign_len = 0;
	info.str = itoa_base_unsigned(val, base);
	if (fmt->has_precision && fmt->precision == 0 && val == 0)
		info.len = 0;
	else
		info.len = ft_strlen(info.str);
	info.pad_zero = 0;
	if (fmt->precision > info.len)
		info.pad_zero = fmt->precision - info.len;
	info.prefix_str = NULL;
	info.prefix_len = 0;
	if (prefix && (val != 0 || use_prefix_when_zero))
	{
		info.prefix_str = prefix;
		info.prefix_len = 2;
	}
	info.total_len = info.prefix_len + info.pad_zero + info.len;
	info.pad_space = max(fmt->width - info.total_len, 0);
	info.total_len += info.pad_space;
	return (info);
}


t_render_info	compute_signed_render_info(t_format *fmt, int val)
{
	t_render_info	info;
	int				neg;
	const char		*base;

	base = "0123456789";
	neg = 0;
	if (val < 0)
	{
		neg = 1;
		val = -val;
	}
	info.sign_char = sign_check(fmt, neg ? -val : val);
	info.sign_len = 0;
	if (info.sign_char != '\0')
		info.sign_len = 1;
	info.str = itoa_base(val, base);
	if (fmt->has_precision && fmt->precision == 0 && val == 0)
		info.len = 0;
	else
		info.len = ft_strlen(info.str);
	info.pad_zero = 0;
	if (fmt->precision > info.len)
		info.pad_zero = fmt->precision - info.len;
	info.total_len = info.sign_len + info.pad_zero + info.len;
	info.pad_space = max(fmt->width - info.total_len, 0);
	info.total_len += info.pad_space;
	return (info);
}
