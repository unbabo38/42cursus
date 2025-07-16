#include "../../include/so_long.h"

void	check_shape(const t_game *game)
{
	int	i;
	int	line_length;
	int	expected_width;

	expected_width = ft_strlen(game->map.data[0]);
	i = 1;
	while (game->map.data[i])
	{
		line_length = ft_strlen(game->map.data[i]);
		if (line_length != expected_width)
		{
			free_all(game);
			error_exit("map is not rectangular\n");
		}
		i++;
	}
}

int	has_ber_extension(const char *filename)
{
    size_t len;
	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	return (ft_strcmp(filename + len - 4, ".ber") == 0);
}

int open_file(const char *filename)
{
	int fd;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	return (fd);
}
char	*line_to_lines(const int fd)
{
	char	*line;
	char	*lines;
	char	*attached;

	line = NULL;
	lines = ft_strdup("");
	while (1)
	{

		line = get_next_line(fd);
		if (!line)
			break;
		attached = ft_strjoin(lines, line);

		free(lines);
		lines = attached;
		free(line);
	}
	return(lines);
}

void	read_line_to_map(t_game *game, char *filename)
{
	int		fd;
	char	*lines;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		error_exit("open failed\n");
	lines = line_to_lines(fd);
	close(fd);
	if (!lines)
		error_exit("read map lines failed\n");
	game->map.data = ft_split(lines, '\n');
	free(lines);
	if (!game->map.data)
		error_exit("split failed\n");
	game->map.height = 0;
	while (game->map.data[game->map.height])
		game->map.height++;
	game->map.width = ft_strlen(game->map.data[0]);
	if (game->map.width == 0 || game->map.height == 0)
		error_exit("invalid map size\n");
}
