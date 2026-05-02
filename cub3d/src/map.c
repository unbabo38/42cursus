#include "../include/cub3d.h"


void fill_space(char *dst, char *src, t_data *data)
{
	int	i = 0;
	if (!src || !dst)
		return ;
	while(src && src[i]  && src[i] != '\n' && i < data->map_width)
	{
		dst[i] = src[i];
		i++;
	}

	while (i < data->map_width)
	{
		dst[i] = ' ';
		i++;
	}
	dst[i] = '\0';
}

void make_map(char ***map, int fd, t_data *data)
{
	int 	i = 0;
	char 	*line;
	*map = malloc(sizeof(char *) * (data->map_height + 1));
	if (!*map)
		return ;
	while(i < data->map_height)
	{

		line = get_next_line(fd);
		(*map)[i] = malloc(sizeof(char) * (data->map_width + 1));
		if (!(*map)[i])
			break;
		fill_space((*map)[i], line, data);
		free(line);
		i++;
	}

	(*map)[i] = NULL;
}

int	map_init(t_data *data)
{
	const char *filename = "maps/validmap.cub";
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open file");
		return 1;
	}
	char *line;
	while((line = get_next_line(fd)))
	{
		int	width = ft_strlen(line);
		if (width > 0 && line[width - 1] == '\n')
            width--;
		if (data->map_width < width)
			data->map_width = width;
		free(line);
		data->map_height++;
	}
	//printf("DEBUG: map_height = %d, map_width = %d\n", data->map_height, data->map_width);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
        return 1;
	make_map(&(data->map), fd, data);
	printf("First row check: %s\n", data->map[0]);
	close(fd);

	return 0;
}
