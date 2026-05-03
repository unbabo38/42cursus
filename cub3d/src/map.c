#include "../include/cub3d.h"

void free_tab(char **tab)
{
    int i;

    if (!tab)
        return;
    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

char **copy_map(t_data *data)
{
    char **copy;
    int i;

    copy = malloc(sizeof(char *) * (data->map_height + 1));
    if (!copy)
        return (NULL);
    i = 0;
    while (i < data->map_height)
    {
        // 既に幅が揃っているので、固定長(map_width)で確保しても良い
        copy[i] = ft_strdup(data->map[i]);
        if (!copy[i])
        {
			free_tab(copy);
            return (NULL);
        }
        i++;
    }
    copy[i] = NULL;
    return (copy);
}

int	check_wall(t_data *data, char **map, int h, int w)
{
	if (h < 0 || h >= data->map_height || w < 0 || w >= data->map_width)
		return (1);
	char position = map[h][w];
	if (position == ' ')
		return (1);
	if (position == '1' || position == 'v')
		return (0);
	map[h][w] = 'v';
	if (check_wall(data, map, h + 1, w ))
		return (1);
	if (check_wall(data, map, h, w + 1))
		return (1);
	if (check_wall(data, map, h - 1, w))
		return (1);
	if (check_wall(data, map, h, w - 1))
		return (1);
	return (0);
}


int	check_characters(t_data *data)
{
	int h = 0;
	int w = 0;
	int cnt = 0;
	while(h < data->map_height)
	{
		w = 0;
		while(w < data->map_width)
		{
			char position = data->map[h][w];
			if (position == 'N' || position == 'S' || position == 'W' || position == 'E')
			{
				data->posX = h + 0.5;
				data->posY = w + 0.5;
				default_dir(position, data);
				data->map[h][w] = '0';
				cnt++;
			} else if (!(position == '0' || position == '1' || position != ' '))
			{
				printf("%c", position);
				return (1);
			}
			w++;
		}
		h++;
	}
	if (cnt != 1)
		return (1);
	else
		return (0);
}

int check_map(t_data *data, char **map)
{
    printf("Debug: Starting check_characters...\n");
    if (check_characters(data) != 0)
    {
        printf("Error: check_characters failed\n");
        return (1);
    }

    printf("Debug: Starting check_wall (Flood Fill)...\n");
    // 小数点が含まれているのでキャストが必要
    if (check_wall(data, map, (int)data->posX, (int)data->posY) != 0)
    {
        printf("mapng: Wall is not closed\n");
        return (1);
    }

    printf("mapok\n");
    return (0);
}

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

void make_map(int fd, t_data *data)
{
	int 	i = 0;
	char 	*line;
	data->map = malloc(sizeof(char *) * (data->map_height + 1));
	if (!data->map)
		return ;
	while(i < data->map_height)
	{

		line = get_next_line(fd);
		data->map[i] = malloc(sizeof(char) * (data->map_width + 1));
		if (!data->map[i])
			break;
		fill_space(data->map[i], line, data);
		free(line);
		i++;
	}

	data->map[i] = NULL;
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
	make_map(fd, data);
	char **cmap = copy_map(data);
	check_map(data, cmap);
	free_tab(cmap); // 使い終わったら必ず解放！
	printf("First row check: %s\n", data->map[0]);
	close(fd);

	return 0;
}
