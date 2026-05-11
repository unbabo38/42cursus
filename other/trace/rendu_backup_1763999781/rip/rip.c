#include <stdio.h>

int is_val(char *str)
{
	int i = 0;
	int open = 0;
	int close = 0;
	while(str[i])
	{
		if (str[i] == '(')
			open++;
		if (str[i] == ')')
		{
			if (open > 0)
				open--;
			else
				close++;
		}
		i++;
	}
	return open + close;
}

void	rip(int diff, int deleted_num, char *str, int pos)
{
	if (diff == deleted_num && !is_val(str))
	{
		printf("%s", str);
		printf("%s", "\n");
		return ;
	}
	for(int i = pos; str[i]; i++)
	{
		if (str[i] == '(' || str[i] == ')')
		{
			char tmp = str[i];
			str[i] = ' ';
			rip(diff, deleted_num + 1, str, pos);
			str[i] = tmp;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		int diff = is_val(argv[1]);
		rip(diff, 0, argv[1], 0);
	}
}
