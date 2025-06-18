#include "./include/push_swap.h"


int	sort_two_or_three(t_list **a)
{
	int len = count_lists(*a);
	if (len == 2)
	{
		if ((*a)->value > (*a)->next->value)
			sa(a);
		return 2;
	}
	else if (len < 2)
		return 1;

	int f = (*a)->value;
	int s = (*a)->next->value;
	int t = (*a)->next->next->value;

	if (f < s && s < t)
		return 1;
	else if (f > s && s < t && f < t)
		sa(a);
	else if (f > s && s > t)
	{
		sa(a);
		rra(a);
	}
	else if (f > s && s < t && f > t)
		ra(a);
	else if (f < s && s > t && f < t)
	{
		sa(a);
		ra(a);
	}
	else if (f < s && s > t && f > t)
		rra(a);
	return (3);
}
