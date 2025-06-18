#include "./include/push_swap.h"

int find_position_a(int val, t_list *a)
{
	t_list *tmp = a;
	int pos = 0;
	int min = min_num(a);
	int max = max_num(a);

	// case1: val is smaller than all → insert before min
	if (val < min)
	{
		while (tmp->value != min)
		{
			tmp = tmp->next;
			pos++;
		}
		return pos;
	}

	// case2: val is greater than all → insert after max
	if (val > max)
	{
		while (tmp->value != max)
		{
			tmp = tmp->next;
			pos++;
		}
		return (pos + 1) % count_lists(a);
	}

	// case3: find spot between nodes (ascending)
	while (tmp && tmp->next)
	{
		if (tmp->value < val && val < tmp->next->value)
			return pos + 1;
		tmp = tmp->next;
		pos++;
	}

	return 0;
}


void push_back_to_a(t_list **a, t_list **b)
{
	while (*b)
	{
		int val = (*b)->value;
		int pos_a = find_position_a(val, *a); // Aにおける挿入位置
		int size_a = count_lists(*a);

		if (pos_a <= size_a / 2)
		{
			while (pos_a-- > 0)
			{
				ra(a);
			}
		}
		else
		{
			pos_a = size_a - pos_a;
			while (pos_a-- > 0)
			{
				rra(a);
			}
		}

		pa(a, b);
	}
}

void rotate_a_to_min(t_list **a)
{
	int min = min_num(*a);
	int pos = 0;
	t_list *tmp = *a;

	// 最小値の位置を探す
	while (tmp && tmp->value != min)
	{
		tmp = tmp->next;
		pos++;
	}

	int size = count_lists(*a);
	if (pos <= size / 2)
	{
		while ((*a)->value != min)
		{
			ra(a);
		}
	}
	else
	{
		while ((*a)->value != min)
		{
			rra(a);
		}
	}
}
