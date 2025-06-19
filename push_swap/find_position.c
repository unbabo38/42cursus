//#include "./include/libft.h"
#include "./include/push_swap.h"

t_list *new_node(int value) {
	t_list *node = malloc(sizeof(t_list));
	if (!node) exit(1);
	node->value = value;
	node->next = NULL;
	return node;
}

void add_back(t_list **head, t_list *new) {
	if (!*head) {
		*head = new;
		return;
	}
	t_list *tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	find_position_b(t_list *a, t_list *b)
{
	int		pos = 0;
	t_list	*tmp = b;
	int		val = a->value;
	int		min = min_num(b);
	int		max = max_num(b);

	////printf("min=%d max=%d\n", min, max);
if (val > max || val < min)
{
	tmp = b;
	pos = 0;
	while (tmp)
	{
		if (tmp->value == max)
			return pos + 1;  // maxの「後」に挿入したいので +1
		tmp = tmp->next;
		pos++;
	}
}


	// case3: val is between two values in descending order
	tmp = b;
	pos = 0;
	while (tmp && tmp->next)
	{
		//////printf("value = %d, postion = %d\n", tmp->value, pos);
		if (tmp->value < val && val < tmp->next->value)
			return (pos + 1);
		pos++;
		tmp = tmp->next;
	}

	// fallback
	return (0);
}







t_cheap calculate(t_list *a, t_list *b)
{
	t_list *tmp = a;
	t_cheap best = { .cost = INT_MAX };
	int size_b = count_lists(a);
	int size_a= count_lists(b);
	int idx = 0;

	////printf("Entering calculate(): size_a = %d, size_b = %d\n", size_a, size_b);

	while (tmp)
	{
		////printf("  [idx = %d] tmp->value = %d\n", idx, tmp->value);

		int position_a = find_position_b(tmp, b);
		////printf("    position_a = %d\n", position_a);

		int rb = idx;
		int rrb = size_b - idx;
		int ra = position_a;
		int rra = size_a - ra;

		int c1 = max(ra, rb);
		int c2 = max(rra, rrb);
		int c3 = ra + rrb;
		int c4 = rb + rra;
		int current_cost = min4(c1, c2, c3, c4);
		int pat = (current_cost == c1 ? 1 : current_cost == c2 ? 2 : current_cost == c3 ? 3 : 4);

		////printf("    Costs: c1=%d c2=%d c3=%d c4=%d => min=%d (pat=%d)\n", c1, c2, c3, c4, current_cost, pat);

		if (current_cost < best.cost)
		{
			best.idx = idx;
			best.ra = ra;
			best.rb = rb;
			best.pat = pat;
			best.cost = current_cost;
			best.node = tmp;
			////printf("    >>> best updated: idx=%d, ra=%d, rb=%d, cost=%d, pat=%d\n", idx, ra, rb, current_cost, pat);
		}

		if (best.cost == 1)
		{
			////printf("    <<< early break (cost==1)\n");
			break;
		}
		tmp = tmp->next;
		idx++;
	}

	////printf("Leaving calculate(): best.idx=%d, cost=%d, pat=%d\n", best.idx, best.cost, best.pat);
	return best;
}


void rotate_a_to_max(t_list **b)
{
	int max = max_num(*b);
	int pos = 0;
	t_list *tmp = *b;

	// 最小値の位置を探す
	while (tmp && tmp->value != max)
	{
		tmp = tmp->next;
		pos++;
	}

	int size = count_lists(*b);
	if (pos <= size / 2)
	{
		while ((*b)->value != max)
		{
			rb(b);
		}
	}
	else
	{
		while ((*b)->value != max)
		{
			rrb(b);
		}
	}
}

void execute_move(t_list **a, t_list **b, t_cheap mv) {
    int na = count_lists(*b);
    int nb = count_lists(*a);

    int rra_a = na - mv.ra;
    int rrb_b = nb - mv.rb;
    int m;
    switch (mv.pat) {
      case 1: // rr
        m = min(mv.ra, mv.rb);
        for (int i=0; i<m; i++) { rr(a, b);}
        for (int i=m; i<mv.ra; i++) ra(b);
        for (int i=m; i<mv.rb; i++) rb(a);
        break;
      case 2: // rrr
        m = min(rra_a, rrb_b);
        for (int i=0; i<m; i++) { rrr(a, b); }
        for (int i=m; i<rra_a; i++) rra(b);
        for (int i=m; i<rrb_b; i++) rrb(a);
        break;
      case 3: // ra + rrb
		////printf("rrb_b: %d\n", rrb_b);
        for (int i=0; i<mv.ra;   i++) ra(b);
        for (int i=0; i<rrb_b;    i++) rrb(a);
        break;
      case 4: // rra + rb
        for (int i=0; i<rra_a;    i++) rra(b);
        for (int i=0; i<mv.rb;    i++) rb(a);
        break;
    }

    // (3) 最後にプッシュ
    pa(b, a);

	t_list *tmpa = *a;
	t_list *tmpb = *b;
	while (tmpa)
	{
		////printf("%d,", tmpa->value);
		tmpa = tmpa->next;
	}
	////printf("\n");
	while (tmpb)
	{
		////printf("%d,", tmpb->value);
		tmpb = tmpb->next;
	}

}
