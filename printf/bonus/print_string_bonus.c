#include "ft_printf_bonus.h"

int print_string(t_format *fmt, const char *string)
{
	t_render_info info = {0};
	if (!string)
		info.str = "(null)";
	else
		info.str = (char *)string;
	info.len = ft_strlen(info.str);
	if (fmt->has_precision)
	{
		if (fmt->precision == 0)
			info.len = 0;
		else if (!string && fmt->precision <= 5)
			info.len = 0;
		else if (fmt->precision < info.len)
			info.len = fmt->precision;
	}
	int pad_space = fmt->width - info.len;
	if (pad_space > 0)
		info.pad_space = pad_space;
	else
		info.pad_space = 0;
	info.total_len = info.len + info.pad_space;
	render_output(fmt, &info);
	return (info.total_len);
}
