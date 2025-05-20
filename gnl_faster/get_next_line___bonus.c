#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//extern int g_current_malloc_fail_index;

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

#define MAX_FD 128

// static バッファと状態管理
static int   g_bytes[MAX_FD];
static char  g_buffer[MAX_FD][BUFFER_SIZE > 1000 ? 1000 : BUFFER_SIZE];
static char *g_bufp[MAX_FD];

// 1行分の状態（fdごとに保持）
typedef struct s_string {
	char  *line;
	int   line_length;
	int   saved_size;
} t_string;

static t_string g_strs[MAX_FD];  // FDごとの状態保存

void	free_and_reset(t_string *str)
{
	if (str->line)
	{
		free(str->line);
		str->line = NULL;
	}
	str->line_length = 0;
	str->saved_size = 0;
}

void	reset_fd_state(int fd)
{
	if (fd >= 0 && fd < MAX_FD)
	{
		g_bytes[fd] = 0;
		g_bufp[fd] = NULL;
		for (size_t i = 0; i < sizeof(g_buffer[fd]); i++)
			g_buffer[fd][i] = 0;
	}
}

int	ft_getchar(const int fd)
{
	if (fd < 0 || fd >= MAX_FD)
		return (-1);
	if (g_bytes[fd] == 0)
	{
		int r = read(fd, g_buffer[fd], BUFFER_SIZE);
		if (r < 0)
			return (-42);
		if (r == 0)
			return (EOF);
		g_bytes[fd] = r;
		g_bufp[fd] = g_buffer[fd];
	}
	g_bytes[fd]--;
	return (*g_bufp[fd]++);
}

int	ft_insert_char_to_line(t_string *str, char c)
{
	if (!str->line)
	{
		//fprintf(stderr, "[GNL] malloc for new line (init)\n");
		str->saved_size = 16;
		str->line = malloc(sizeof(char) * str->saved_size);
		if (!str->line)
		{
			//fprintf(stderr, "[GNL] malloc failed for new line!\n");
			return (-1);
		}
	}
	if (str->line_length + 2 >= str->saved_size)
	{
		int new_size = str->saved_size * 2;
		char *new_line = malloc(sizeof(char) * new_size);
		if (!new_line)
		{
			//fprintf(stderr, "[GNL DEBUG] malloc failed at index %d (new_size = %d)\n",g_current_malloc_fail_index, new_size);
			return (-1);
		}
		for (int i = 0; i < str->line_length; i++)
			new_line[i] = str->line[i];
		free(str->line);
		str->line = new_line;
		str->saved_size = new_size;
	}
	str->line[str->line_length++] = c;
	return (0);
}

char	*get_next_line(int fd)
{
	int			c;
	t_string	*str;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	str = &g_strs[fd];
	//fprintf(stderr, "[DEBUG] BEFORE read: line_length=%d, ptr=%p\n",str->line_length, (void *)str->line);
	while (1)
	{
		c = ft_getchar(fd);
		if (c == -42)
		{
			free_and_reset(str);
			reset_fd_state(fd);
			return (NULL);
		}
		if (c == EOF)
			break;
		if (ft_insert_char_to_line(str, c) == -1)
		{
			free_and_reset(str);
			reset_fd_state(fd);
			return (NULL);
		}
		if (c == '\n')
		{
			//fprintf(stderr, "[DEBUG] NL: line_length=%d, ptr=%p\n",str->line_length, (void *)str->line);

			//if (str->line_length == 0 && str->line == NULL)
			//	continue;  // 改行だけの行 → 空行として扱う
			break;
		}
	}
	if (str->line_length > 0 || str->line != NULL)
	{
		if (ft_insert_char_to_line(str, '\0') == -1)
		{
			free_and_reset(str);
			reset_fd_state(fd);
			return (NULL);
		}
		char *ret = strdup(str->line);
		if (!ret)
		{
			//fprintf(stderr, "[GNL DEBUG] strdup failed at malloc index %d (line_length = %d)\n",g_current_malloc_fail_index, str->line_length);			free_and_reset(str);
			free_and_reset(str);
			reset_fd_state(fd);
			return NULL;
		}
		free_and_reset(str);       // ← ここで str->line をちゃんと free
		//reset_fd_state(fd);
		//fprintf(stderr, "[DEBUG] fd=%d: RETURNING line=%p, content=\"%s\"\n",fd, (void *)ret, ret ? ret : "NULL");
		return (ret);
	}
	free_and_reset(str);
	reset_fd_state(fd);
	return (NULL);
}
/*
#include <fcntl.h>   // open
#include <stdio.h>   // printf, perror
#include <stdlib.h>  // free
#include "get_next_line.h"

// 安全な free
void safe_free(char **ptr) {
	if (ptr && *ptr) {
		free(*ptr);
		*ptr = NULL;
	}
}

int main(void)
{
	const char *filename = "multiple_nl.txt";
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return 1;
	}

	char *line;
	int count = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Line %d: ", ++count);
		if (line[0] == '\n' && line[1] == '\0')
			printf("[ONLY NEWLINE] \\n\n");
		else
			printf("%s", line);

		safe_free(&line);
	}
	printf("Done. Total lines read: %d\n", count);

	close(fd);
	return 0;
}
*/
