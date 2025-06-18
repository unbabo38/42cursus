#include "./include/push_swap.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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


int main(int argc, char **argv)
{
	t_list *a = parse_args(argc, argv);
	t_list *b = NULL;

	if (!a || count_lists(a) <= 3)
	{
		sort_two_or_three(&a);
		return 0;
	}



	pb(&a, &b);

	pb(&a, &b);

	while (count_lists(a) > 3) {
	//	//printf("Loop Start\n");
		t_cheap mv = calculate(a, b);
		execute_move(&a, &b, mv);
	}
	//printf("A:\n");
	for (t_list *tmp = a; tmp; tmp = tmp->next)
		//printf("%d ", tmp->value); // valueがある構造体前提
	//printf("\n");
	//printf("B:\n");
	for (t_list *tmp = b; tmp; tmp = tmp->next)
		//printf("%d ", tmp->value); // valueがある構造体前提
	//printf("\n");

	rotate_a_to_max(&b);
	sort_two_or_three(&a);
	push_back_to_a(&a, &b);
	rotate_a_to_min(&a);
/*
	printf("final A:\n");
	for (t_list *tmp = a; tmp; tmp = tmp->next)
		printf("%d ", tmp->value); // valueがある構造体前提
	printf("\n");
	return 0;
*/
	}
