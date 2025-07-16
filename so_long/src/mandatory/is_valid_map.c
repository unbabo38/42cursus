#include "../../include/so_long.h"

char **copy_map(char **src, int height)
{
	char **dst = malloc(sizeof(char *) * (height + 1));
	if (!dst)
		return (NULL);
	for (int i = 0; i < height; i++)
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			while (i-- > 0)
				free(dst[i]);
			free(dst);
			return (NULL);
		}
	}
	dst[height] = NULL;
	return (dst);
}


int	is_valid_path(char **map, int h, int w)
{
	if (map[h][w] == '1' || map[h][w] == 'V')
		return (INVALID_PATH);

	if (map[h][w] == 'P')
		return (VALID_PATH);
	map[h][w] = 'V';

	return (
		is_valid_path(map, h + 1, w) ||
		is_valid_path(map, h - 1, w) ||
		is_valid_path(map, h, w + 1) ||
		is_valid_path(map, h, w - 1)
	);
}

int	is_valid_map(t_game *game)
{
	int h;
	int	w;
	int flg;
	char** dummy_map;

	flg = 1;
	h = 0;
	while(h < game->map.height)
	{
		w = 0;
		while(w < game->map.width)
		{
			if (game->map.data[h][w] == 'C' || game->map.data[h][w] == 'E')
			{
				dummy_map = copy_map(game->map.data, game->map.height);
					if (!dummy_map)
						error_exit("Map copy failed");
				if (!is_valid_path(dummy_map, h, w))
					flg = 0;
				free_map(dummy_map);
			}
			w++;
		}
		h++;
	}
	if (flg)
		return (VALID_MAP);
	free_all(game);
	error_exit("no valid path\n");
	return(INVALID_MAP);
}
