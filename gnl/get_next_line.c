#include "get_next_line.h"
#include <unistd.h>
//#include <stdio.h>
//#include <fcntl.h>


char	*make_line()

char	*get_next_line(int fd)
{
	char *buffers[BUFFER_SIZE + 1];
	static char *served_string;
	ssize_t	line_size =	read(fd, buffers[fd], BUFFER_SIZE);
	if (!current_line)
		return (NULL);
	if (fd < 0)
		return (NULL);
	int i = 0;
	if (strlen(served_string) > 0)
	{
		while(served_string[i] != '\n' || i < line_size)
			i++;
	}
	if (served_string[i] == '\n')
	{
		char *current_line = malloc(sizeof(char) * (i + 1));
		current_line = served_string;
		return (current_line);
	}
	while(buffers[fd][i] != '\n')
	{
		served_string[i] +=  buffers[fd][i];
		i++;
	}
	return (served_string);
}

int	main()
{
	int fd = open("test.txt", O_RDONLY);
}
