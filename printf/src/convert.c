/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:48:48 by tmura             #+#    #+#             */
/*   Updated: 2025/05/30 10:48:58 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	count_digits_unsigned(unsigned long long n, int base_len)
{
	int	digit;

	digit = 0;
	if (n == 0)
		digit = 1;
	while (n > 0)
	{
		digit++;
		n /= base_len;
	}
	return (digit);
}

char	*convert_to_base(unsigned long long n,
			const char *base, int base_len, int digit)
{
	char	*num;

	num = malloc(digit + 1);
	if (!num)
		return (NULL);
	num[digit] = '\0';
	if (n == 0)
		num[0] = '0';
	while (digit-- > 0 && n > 0)
	{
		num[digit] = base[n % base_len];
		n /= base_len;
	}
	return (num);
}

char	*itoa_base_unsigned(unsigned long long n, const char *base)
{
	int		base_len;
	int		digit;

	base_len = ft_strlen(base);
	digit = count_digits_unsigned(n, base_len);
	return (convert_to_base(n, base, base_len, digit));
}

char	*itoa_base(long long n, const char *base)
{
	unsigned long long	abs_val;
	int					base_len;
	int					digit;

	if (n < 0)
		abs_val = (unsigned long long)(-(n + 1)) + 1;
	else
		abs_val = (unsigned long long)n;
	base_len = ft_strlen(base);
	digit = count_digits_unsigned(abs_val, base_len);
	return (convert_to_base(abs_val, base, base_len, digit));
}
