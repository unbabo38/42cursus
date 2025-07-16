#include "../../include/so_long.h"

void	count_elements(t_game *game)
{
	int h = 0;
	while (h < game->map.height)
	{
		int w = 0;
		while (w < game->map.width)
		{
			char tile = game->map.data[h][w];
			if (tile == 'P') {
				game->elements.player_count++;
				game->elements.player_h = h;
				game->elements.player_w = w;
			}
			else if (tile == 'E')
				game->elements.exit_count++;
			else if (tile == 'C')
				game->elements.collectible_count++;
			else if (tile == 'X')
			{
				game->enemy.h = h;
				game->enemy.w = w;
				game->enemy.dir = 1;
			}
			else if (tile != '0' && tile != '1')
			{
				free_all(game);
				error_exit("Invalid character in map");
			}
			w++;
		}
		h++;
	}
}

void	check_elements(t_game *game)
{
	count_elements(game);
	if (game->elements.player_count != 1)
	{
		free_all(game);
		error_exit("map must contain exactly 1 player\n");
	}
	if (game->elements.exit_count < 1)
	{
		free_all(game);
		error_exit("map must contain at least 1 exit\n");
	}
	if (game->elements.collectible_count < 1)
	{
		free_all(game);
		error_exit("map must contain at least 1 collectible\n");
	}
}
