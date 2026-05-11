/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:24:24 by tmura             #+#    #+#             */
/*   Updated: 2025/06/18 11:04:30 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/push_swap.h"
int	swap_top_two_nodes(t_list **node)
{
	t_list	*head;
	t_list	*second;

	if (!node || !*node || !(*node)->next)
		return (0);
	head = *node;
	second = (*node)->next;
	head->next = second->next;
	second->next = head;
	*node = second;
	return (1);
}

void	sa(t_list **a)
{
	if (swap_top_two_nodes(a))
	{
		printf("sa\n");
	}
}

void	sb(t_list **b)
{
	if (swap_top_two_nodes(b))
	{
		printf("sb\n");
	}
}

void	ss(t_list **a, t_list **b)
{
	t_list	*head;
	t_list	*second;
	int		success_swap_a;
	int		success_swap_b;

	success_swap_a = swap_top_two_nodes(a);
	success_swap_b = swap_top_two_nodes(b);
	if (success_swap_a || success_swap_b)
	{
		printf("ss\n");
	}
}
