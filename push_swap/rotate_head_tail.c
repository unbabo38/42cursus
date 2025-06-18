/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_head_tail.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:23:12 by tmura             #+#    #+#             */
/*   Updated: 2025/06/18 11:04:32 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/push_swap.h"
t_list *ft_lstlast(t_list *lst)
{
	if (!lst)
		return NULL;
	while (lst->next)
		lst = lst->next;
	return lst;
}
int	rotate_head_tail(t_list **node)
{
	t_list	*tail;
	t_list	*head;

	if (!node || !*node || !(*node)->next)
		return (0);
	head = *node;
	*node = head->next;
	head->next = NULL;
	tail = ft_lstlast(*node);
	tail->next = head;
	return (1);
}

void	ra(t_list **a)
{
	if (rotate_head_tail(a))
	{
		printf("ra\n");
	}
}

void	rb(t_list **b)
{
	if (rotate_head_tail(b))
	{
		printf("rb\n");
	}
}

void	rr(t_list **a, t_list **b)
{
	int	rotate_head_tail_a;
	int	rotate_head_tail_b;

	rotate_head_tail_a = rotate_head_tail(a);
	rotate_head_tail_b = rotate_head_tail(b);
	if (rotate_head_tail_a || rotate_head_tail_b)
	{
		printf("rr\n");
	}
}
