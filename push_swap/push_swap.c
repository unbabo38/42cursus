#include "./include/push_swap.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdlib.h>

int	*find_lis_indexes(t_list *a, int *lis_len)
{
	int size = count_lists(a);
	int *arr = malloc(sizeof(int) * size);
	int *lengths = malloc(sizeof(int) * size);
	int *prev = malloc(sizeof(int) * size);
	int *lis_indexes = NULL;

	if (!arr || !lengths || !prev)
		return NULL;

	t_list *tmp = a;
	for (int i = 0; tmp; i++, tmp = tmp->next)
		arr[i] = tmp->value;

	for (int i = 0; i < size; i++) {
		lengths[i] = 1;
		prev[i] = -1;
	}

	int max_len = 1, max_end = 0;
	for (int i = 1; i < size; i++) {
		for (int j = 0; j < i; j++) {
			if (arr[j] < arr[i] && lengths[j] + 1 > lengths[i]) {
				lengths[i] = lengths[j] + 1;
				prev[i] = j;
			}
		}
		if (lengths[i] > max_len) {
			max_len = lengths[i];
			max_end = i;
		}
	}

	*lis_len = max_len;
	lis_indexes = malloc(sizeof(int) * max_len);
	if (!lis_indexes)
		return NULL;

	int k = max_end;
	for (int i = max_len - 1; i >= 0; i--) {
		lis_indexes[i] = k;
		k = prev[k];
	}

	free(arr);
	free(lengths);
	free(prev);
	return lis_indexes;
}

int is_in_lis(int value, int *lis, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (lis[i] == value)
			return 1;
	}
	return 0;
}

t_list *parse_args(int argc, char **argv)
{
	t_list *a = NULL;

	for (int i = 1; i < argc; i++)
	{
		int	val = atoi(argv[i]);

		t_list *node = new_node(val);
		if (!node)
			exit(1);
		add_back(&a, node);  // ← ここで使う！
	}
	return a;
}

void mark_lis_nodes(t_list *a)
{
	int len;
	int *indexes = find_lis_indexes(a, &len);
	if (!indexes)
		return;

	t_list *tmp = a;
	int i = 0;
	while (tmp)
	{
		tmp->keep = 0;
		for (int j = 0; j < len; j++) {
			if (i == indexes[j])
			{
				tmp->keep = 1;
				break;
			}
		}
		tmp = tmp->next;
		i++;
	}
	free(indexes);
}

int exists_unmarked(t_list *a)
{
	while (a)
	{
		//////printf("%d\n", a->keep);
		if (a->keep == 0)
			return (1);
		a = a->next;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_list *a = parse_args(argc, argv);
	t_list *b = NULL;

	if (!a || count_lists(a) <= 3)
	{
		sort_two_or_three(&a);
		return 0;
	}
	mark_lis_nodes(a);


int i = 0;
while (i < 2 && exists_unmarked(a)) {
	if (a->keep == 0) {
		pb(&a, &b);
		i++;
	} else {
		ra(&a);
	}
}

int step = 0;
while (exists_unmarked(a)) {
	if (step++ > 1000)
		break; // 念のため無限回避用
	//////printf("step %d, top = %d, keep = %d\n", step++, a->value, a->keep);
	if (a->keep == 0)
		pb(&a, &b);
	else
	{
		//////printf("step %d, top = %d, keep = %d\n", step++, a->value, a->keep);
		ra(&a);
	}

}
	rotate_a_to_min(&a);
	//printf("A:\n");
	for (t_list *tmp = a; tmp; tmp = tmp->next)
		//printf("%d ", tmp->value); // valueがある構造体前提
	//printf("\n");

	//printf("B :\n");
	for (t_list *tmp = b; tmp; tmp = tmp->next)
		//printf("%d ", tmp->value); // valueがある構造体前提
	//printf("\n");
	//rotate_a_to_max(&b);
	while (count_lists(b))
	{
		t_cheap mv = calculate(b, a);
		execute_move(&b, &a, mv);
	}


	//sort_two_or_three(&a);
	//push_back_to_a(&a, &b);
	rotate_a_to_min(&a);

	//printf("final A:\n");
	for (t_list *tmp = a; tmp; tmp = tmp->next)
		//printf("%d ", tmp->value); // valueがある構造体前提
	//printf("\n");
	return 0;

	}
