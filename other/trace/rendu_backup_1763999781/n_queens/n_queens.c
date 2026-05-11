#include <stdio.h>
#include <stdlib.h>

int check(int *positions, int current_col, int current_row)
{
	for(int prev_col = 0; prev_col < current_col; prev_col++)
	{
		int prev_row = positions[prev_col];
		if (prev_row == current_row ||
			prev_row - prev_col == current_row - current_col ||
			prev_row + prev_col == current_row + current_col)
			return (0);
	}
	return (1);

}

void solve(int *positions, int col, int n)
{
	if (col == n)
	{
		for (int i = 0; i < n; i++)
		{
			if (i > 0)
				printf(" ");
			printf("%d", positions[i]);
		}
		printf ("\n");
		return ;
	}
	for(int row = 0; row < n; row++)
	{
		if (check(positions, col, row))
		{
			positions[col] = row;
			solve(positions, col + 1, n);
		}
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);

	int n = atoi(argv[1]);
	if (n <= 0)
        return (0);
	int *positions = malloc(sizeof(int) * n);
	solve(positions, 0, n);
	return (0);
}
