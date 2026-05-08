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
	if (position == '1' || position == 'v')
		return (0);
	else if (position == '0')
		map[h][w] = 'v';
	else
		return 1;

	if (check_wall(data, map, h + 1, w) ||
        check_wall(data, map, h - 1, w) ||
        check_wall(data, map, h, w + 1) ||
        check_wall(data, map, h, w - 1))
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
			printf("%c", position);
			//printf("map_contents:%c\n", data->map[h][w]);
			if (position == 'N' || position == 'S' || position == 'W' || position == 'E')
			{
				data->posX = w + 0.5;
				data->posY = h + 0.5;
				default_dir(position, data);
				data->map[h][w] = '0';
				cnt++;
			}
			if (position == 'K')
			{
				data->sprite.x = w + 0.5;
				data->sprite.y = h + 0.5;
				data->map[h][w] = '0';
				data->sprite.exist = 1;
			}
			// } else if (!(position == '0' || position == '1' || position != ' '))
			// {
			// 	printf("out_contents:%c\n", position);
			// 	return (1);
			// }
			w++;
		}
		printf("\n");
		h++;
	}
	printf("%d", cnt);
	if (cnt != 1)
		return (1);
	else
		return (0);
}

int check_map(t_data *data)
{

    if (check_characters(data) != 0)
    {
		free_exit(data, 1, "Error\ncheck_characters failed\n");
        return (1);
    }
	char **cmap = copy_map(data);

	if (check_wall(data, cmap, (int)data->posY, (int)data->posX) != 0)
    {
		free_tab(cmap);
		free_exit(data, 1, "Wall is not closed\n");
        return (1);
    }
	free_tab(cmap);

    printf("mapok\n");
    return (0);
}

void fill_space(char *dst, char *src, t_data *data)
{
    int i = 0;

    if (!src || !dst) return;
    // 1. srcの内容をコピー（改行とヌル文字に当たるまで）
    while (src[i] && src[i] != '\n' && i < data->map_width)
    {
        dst[i] = src[i];
        i++;
    }
    // 2. 残りの幅をすべて '1' (壁) で埋める
    while (i < data->map_width)
    {
        dst[i] = ' ';
        i++;
    }
    // 3. 最後にヌル文字
    dst[i] = '\0';
}
void make_map(int fd, t_data *data)
{
    int     i = 0;
    char    *line;
    int     in_map = 0;
	int		parse_result = 0;
    data->map = malloc(sizeof(char *) * (data->map_height + 1));
    if (!data->map)
	{
        return ;
	}
	while ((line = get_next_line(fd)))
	{
		if (!in_map)
		{
			parse_result = parse_config_line(line, data);
			if (parse_result == 0)
				in_map = 1;
		}

		// if (!in_map) else にせず、in_mapになった瞬間この下の処理を通す
		if (in_map && i < data->map_height)
		{
			data->map[i] = malloc(sizeof(char) * (data->map_width + 1));
			if (data->map[i])
				// 【重要】trimmedではなく、get_next_lineで取ったままの line を渡す
				fill_space(data->map[i], line, data);
			i++;
		}
		free(line);
	}
    data->map[i] = NULL;
}

int	all_dir_texture_is_valid(t_data *data)
{
	int	cnt = 0;
	if (data->texture.no_path)
		cnt++;
	if (data->texture.so_path)
		cnt++;
	if (data->texture.we_path)
		cnt++;
	if (data->texture.ea_path)
		cnt++;
	if (data->texture.floor_color)
	{
		printf("floor color=%i\n", data->texture.floor_color);
		cnt++;
	}

	if (data->texture.ceiling_color)
		cnt++;
	// if (data->map)
	// 	cnt++;
	printf("cnt=%i\n", cnt);
	return (cnt);
}
int is_empty_line(char *line)
{
    int i = 0;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
            return (0);
        i++;
    }
    return (1); // 空行
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
	int  in_map = 0;
	int		parse_result = 0;
	int		tmp = 0;
	int		is_empty = 0;
	int		map_finished = 0;
	while((line = get_next_line(fd)))
	{
		is_empty = is_empty_line(line);
		if (!in_map)
        {
			parse_result = parse_config_line(line, data);
			if (parse_result == 0)
			{
					in_map = 1;
			}
			if (parse_result == 2)
			{
				tmp = parse_result;
			}
        }

        if (in_map)
        {
			if (is_empty)
			{
				map_finished = 1;
			}
			else if (map_finished)
			{
					write(2, "empty line found in the map!\n", 29);
					tmp = 2;
			}
			else
			{
	            int width = ft_strlen(line);
	            if (width > 0 && line[width - 1] == '\n') width--;
	            if (data->map_width < width) data->map_width = width;
	            data->map_height++;
			}
	   }
		free(line);
	}
	printf("mapheight:%d", data->map_height);
	if (all_dir_texture_is_valid(data) !=6)
	{
		free_exit(data, 1, "lack of info\n");
	}
	if (tmp == 2)
	{
		free_exit(data, 1, "config error\n");
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
        return 1;

	make_map(fd, data);
	// for (int i = 0; i < data->map_height; i++)
	// {
	// 	for (int j = 0; j < data->map_width; j++)
	// 	{
	// 		printf("%c", data->map[i][j]);
	// 	}
	// 	printf("\n");
	// }
	check_map(data);
	close(fd);
	return 0;
}
