#include "get_next_line.h"


#include "get_next_line.h"

char *get_next_line(int fd)
{
	static char *served_string[4064];
	char buffer[BUFFER_SIZE + 1];
	ssize_t bytes_read = 1;
	char *line;
	char *rest;
	int i = 0;

	if (fd < 0 || fd >= 4064 || BUFFER_SIZE <= 0)
		return NULL;

	// 読み込み（改行が出るまで）
	while ((!served_string[fd] || !ft_strchr(served_string[fd], '\n')) &&
		   (bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		char *tmp;
		if (!served_string[fd])
			tmp = ft_strdup(buffer);  // 初期NULL対応
		else
			tmp = ft_strjoin(served_string[fd], buffer);

		if (!tmp)
			return (free(served_string[fd]), served_string[fd] = NULL, NULL);

		free(served_string[fd]);
		served_string[fd] = tmp;
	}

	// 読み込みエラー or EOF + 何もない
	if (bytes_read < 0 || !served_string[fd] || served_string[fd][0] == '\0')
	{
		free(served_string[fd]);
		served_string[fd] = NULL;
		return NULL;
	}

	// 改行または終端までの長さを数える
	while (served_string[fd][i] && served_string[fd][i] != '\n')
		i++;
	if (served_string[fd][i] == '\n')
		i++;

	line = ft_substr(served_string[fd], 0, i);
	if (!line)
		return (free(served_string[fd]), served_string[fd] = NULL, NULL);

	rest = ft_strdup(served_string[fd] + i);
	free(served_string[fd]);
	served_string[fd] = rest;

	return line;
}

/*
int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	int fd2 = open("test2.txt", O_RDONLY);
	char *line;

	line = get_next_line(fd2);
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	return 0;
}
*/
