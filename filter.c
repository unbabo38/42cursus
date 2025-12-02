#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

int	main(int argc, char **argv)
{
	if (argc !=2)
	{
		perror("Error");
		return (1);
	}
	char *str = malloc(strlen(argv[1]) + 1);
	int t = 0;
	while(argv[1][t])
	{
		str[t]  = argv[1][t];
		t++;
	}
	str[t] = '\0';

	int	b_read = 0;
	char *tmp = malloc(100000000);
	char buffer[BUFFER_SIZE];
	int now = 0;
	while(1)
	{
		b_read = read(0, buffer, BUFFER_SIZE);
		if (b_read == 0)
			break;
		for (int i = 0; i < b_read; i++)
			tmp[now++] = buffer[i];
		tmp[now] = '\0';
	}
	int check = 1;
	int i = 0;
	while(now > i + strlen(argv[1]))
	{
		for (size_t j = 0; j < strlen(argv[1]); j++)
		{
			if (tmp[i + j] != str[j])
			{
				check = 0;
				break;
			}
		}
		if (check)
		{
			for (int j = 0; j < strlen(argv[1]); j++)
			{
				tmp[i + j] = '*';
			}
		}
		check=1;
		i++;
	}

	printf("%s", tmp);
	return(0);
}
