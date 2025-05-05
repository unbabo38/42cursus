/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 21:31:21 by tmura             #+#    #+#             */
/*   Updated: 2025/05/05 01:54:47 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>
#include <stdio.h>

int	ft_isprint(int c)
{
	if (32 <= c && c <= 126)
		return (16384);
	else
		return (0);
}
/*
int main(void) {
	printf("%d\n",isprint('A'));
	printf("%d\n",isprint('	'));
	printf("%d\n",ft_isprint('A'));
	printf("%d\n",ft_isprint('	'));
}
*/
