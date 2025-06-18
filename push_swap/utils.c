/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:28:24 by tmura             #+#    #+#             */
/*   Updated: 2025/06/18 11:04:27 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/push_swap.h"

int	max(int left, int right)
{
	if (left >= right)
		return (left);
	else
		return (right);
}

int	min(int left, int right)
{
	if (left <= right)
		return (left);
	else
		return (right);
}

int	min4(int a, int b, int c, int d)
{
	if (min(a, b) <= min(c, d))
		return (min(a, b));
	else
		return (min(c, d));
}

int	count_lists(t_list *nodes)
{
	int	count = 0;
	while (nodes)
	{
		count++;
		nodes = nodes->next;
	}
	return count;
}
