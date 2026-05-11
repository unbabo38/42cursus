#include "./include/push_swap.h"

int	min_num(t_list *node)
{
	int	min_num = INT_MAX;
	while(node)
	{
		if (node->value < min_num)
			min_num = node->value;
		node = node->next;
	}
	return (min_num);
}

int max_num(t_list *node)
{
	if (!node)
		return INT_MIN;

	int max = node->value;

	while (node)
	{
		if (node->value > max)
			max = node->value;
		node = node->next;
	}
	return max;
}
