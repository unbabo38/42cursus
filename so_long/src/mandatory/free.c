#include "../../include/so_long.h"
#include "../../minilibx-linux/mlx.h"

void	free_map(char **map)
{
	int i = 0;
	if (!map)
		return;
	while (map[i])
		free(map[i++]);
	free(map);
}
void	free_all(const t_game *game)
{
	if (!game)
		return;

	free_map(game->map.data);

	if (game->mlx)
	{
		if (game->img.wall)
			mlx_destroy_image(game->mlx, game->img.wall);
		if (game->img.floor)
			mlx_destroy_image(game->mlx, game->img.floor);
		if (game->img.player[0])
			mlx_destroy_image(game->mlx, game->img.player[0]);
		if (game->img.player[1])
			mlx_destroy_image(game->mlx, game->img.player[1]);
		if (game->img.exit)
			mlx_destroy_image(game->mlx, game->img.exit);
		if (game->img.fire)
			mlx_destroy_image(game->mlx, game->img.fire);
		if (game->img.collectible)
			mlx_destroy_image(game->mlx, game->img.collectible);
		if (game->img.enemy)
			mlx_destroy_image(game->mlx, game->img.enemy);
		if (game->win)
			mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}

int	close_game(t_game *game)
{
	free_all(game);
	exit(EXIT_SUCCESS);
	return (0);
}
