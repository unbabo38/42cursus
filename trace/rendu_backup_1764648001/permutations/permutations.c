#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void sort(char *str, int n)
{
	char tmp;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (str[i] > str[j])
			{
				tmp = str[i];
				str[i] = str[j];
				str[j] = tmp;
			}
		}
	}
}

void permutate(char *str, int *used, int depth, int n, char *result)
{
	if (depth == n)
	{
		result[n] = '\0';
		printf("%s\n", result);
		return ;
	}
	for (int i = 0; i < n; i++)
	{
		if (!used[i])
		{
			used[i] = 1;
			result[depth] = str[i];
			permutate(str, used, depth + 1, n, result);
			used[i] = 0;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	int n = strlen(argv[1]);
	char *result = malloc(n + 1);
	int *used = calloc(n, sizeof(int)); // ← すべて0になる
	char *str = argv[1];
	sort(str, n);
	permutate(str, used, 0, n, result);

}
