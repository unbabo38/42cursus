#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

#include <limits.h>
//# include "./libft.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct s_node {
	int value;
	int keep; // たとえばLIS判定用
	struct s_node *next;
} t_list;
typedef struct cheap
{
	t_list *node;
	int     cost;
	int     idx;   // ← idx_a → idx に統一
	int     ra;
	int     rb;
	int     pat;   // ← これが必要
}	t_cheap;


t_list *new_node(int value);
void add_back(t_list **head, t_list *new);
int	min_num(t_list *node);
int	max_num(t_list *node);
int	max(int left, int right);
int	min(int left, int right);
int	min4(int a, int b, int c, int d);
void sa(t_list **a);
void sb(t_list **b);
void ss(t_list **a, t_list **b);
void pa(t_list **a, t_list **b);
void pb(t_list **a, t_list **b);
void ra(t_list **a);
void rb(t_list **b);
void rr(t_list **a, t_list **b);
void rra(t_list **a);
void rrb(t_list **b);
void rrr(t_list **a, t_list **b);
int find_position_b(t_list *a, t_list *b);
t_cheap calculate(t_list *a, t_list *b);
void execute_move(t_list **a, t_list **b, t_cheap mv);
int	count_lists(t_list *nodes);
void free_stack(t_list **stack);
int get_value(t_list *node);
int	sort_two_or_three(t_list **a);
void	push_back_to_a(t_list **a, t_list **b);
void rotate_a_to_min(t_list **a);
void rotate_a_to_max(t_list **b);


#endif
