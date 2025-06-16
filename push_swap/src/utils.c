#include "../include/push_swap.h"

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
