#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

typedef struct pair
{
	int value;
	int	index;
	int	is_lsi;
	int	cost;
}	i_pair;

typedef struct s_node {
    int value;
    int is_lis;
    struct s_node *next;
} t_node;


int	max(int left, int right);
int	min(int left, int right);
int rotate_down(i_pair **a, int b);
int rotate_up(i_pair **a, int b);
int	min4(int a, int b, int c, int d);



#endif
