#include <stdio.h>
#include <stdlib.h>

void	check_subset(int n, int target, int *nums, int *subset, int idx, int subnum, int sum)
{
	if (idx == n)
	{
		if (sum == target)
		{
			for(int i = 0; i < subnum; i++)
			{
				if (i > 0)
					printf(" ");
				printf("%d", subset[i]);
			}
			printf("\n");
		}
		return;
	}

	check_subset(n, target, nums, subset, idx + 1, subnum, sum);
	subset[subnum] = nums[idx];
	check_subset(n, target, nums, subset, idx + 1, subnum + 1, sum + nums[idx]);
}

int	main(int argc, char **argv)
{
	if (argc < 3)
		return (1);

	int	n = argc - 2;
	int target = atoi(argv[1]);
	int	subset[n];
	int nums[n];
	for (int i = 0; i < n; i++)
		nums[i] = atoi(argv[i + 2]);
	check_subset(n, target, nums, subset, 0, 0, 0);
}
