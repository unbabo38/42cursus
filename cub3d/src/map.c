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
	printf("mapheight:%d\n", data->map_height);

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
	printf("mapheight:%d\n", data->map_height);
	while(h < data->map_height)
	{
		w = 0;
		while(w < data->map_width)
		{
			char position = data->map[h][w];
			//printf("map_contents:%c\n", data->map[h][w]);
			if (position == 'N' || position == 'S' || position == 'W' || position == 'E')
			{
				data->posX = w + 0.5;
				data->posY = h + 0.5;
				default_dir(position, data);
				data->map[h][w] = '0';
				cnt++;
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
        dst[i] = '1';
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

    data->map = malloc(sizeof(char *) * (data->map_height + 1));
    if (!data->map)
	{
        return ;
	}
	while ((line = get_next_line(fd)))
	{
		if (!in_map)
		{
			// 0が返ってきたら「そこからマップ」
			if (parse_config_line(line, data) == 0)
				in_map = 1;
		}

		// if (!in_map) else にせず、in_mapになった瞬間この下の処理を通す
		if (in_map && i < data->map_height)
		{
			data->map[i] = malloc(sizeof(char) * (data->map_width + 1));
			if (data->map[i])
				// 【重要】trimmedではなく、get_next_lineで取ったままの line を渡す
				fill_space(data->map[i], line, data);
			printf("Row[%d] Original: |%s|\n", i, line);
			printf("Row[%d] Stored  : |%s|\n", i, data->map[i]);
			i++;
		}
		free(line);
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
	int  in_map = 0;
	while((line = get_next_line(fd)))
	{
		if (!in_map)
        {
            // 設定行として解析。0が返ってきたらそこからマップ！
            if (parse_config_line(line, data) == 0)
			{

				in_map = 1;
			}
        }

        if (in_map)
        {
            // ここからは今までのサイズ計測ロジック
            int width = ft_strlen(line);
            if (width > 0 && line[width - 1] == '\n') width--;
            if (data->map_width < width) data->map_width = width;
            data->map_height++;
        }
		free(line);
	}
	printf("map height:%d\n", data->map_height);
	printf("map width:%d\n", data->map_width);

	//printf("DEBUG: map_height = %d, map_width = %d\n", data->map_height, data->map_width);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
        return 1;
	printf("mapheight2:%d\n", data->map_height);

	make_map(fd, data);
	for (int i = 0; i < data->map_height; i++)
	{
		for (int j = 0; j < data->map_width; j++)
		{
			printf("%c", data->map[i][j]);
		}
		printf("\n");
	}
	char **cmap = copy_map(data);
	check_map(data, cmap);

	free_tab(cmap); // 使い終わったら必ず解放！
	printf("First row check: %s\n", data->map[0]);
	close(fd);

	return 0;
}
