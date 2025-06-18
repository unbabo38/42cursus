/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_tail_head.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:21:22 by tmura             #+#    #+#             */
/*   Updated: 2025/06/18 11:04:33 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/push_swap.h"

int	rotate_tail_head(t_list **node)
{
	t_list	*tail;
	t_list	*head;
	t_list	*prev;

	if (!node || !*node || !(*node)->next)
		return (0);
	head = *node;
	tail = *node;
	prev = NULL;
	while (tail->next)
	{
		prev = tail;
		tail = tail->next;
	}
	prev->next = NULL;
	tail->next = head;
	*node = tail;
	return (1);
}

void	rra(t_list **a)
{
	if (rotate_tail_head(a))
	{
		printf("rra\n");
	}
}

void	rrb(t_list **b)
{
	if (rotate_tail_head(b))
	{
		printf("rrb\n");
	}
}

void	rrr(t_list **a, t_list **b)
{
	int	is_rotated_a;
	int	is_rotated_b;

	is_rotated_a = rotate_tail_head(a);
	is_rotated_b = rotate_tail_head(b);
	if (is_rotated_a || is_rotated_b)
	{
		printf("rrr\n");
	}
}
