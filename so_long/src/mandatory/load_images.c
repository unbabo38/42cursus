#include "../../include/so_long.h"
#include "../../minilibx-linux/mlx.h"

void	load_images(t_game *game)
{
	int	width;
	int	height;
	
	game->img.wall = mlx_xpm_file_to_image(game->mlx, "img/wall.xpm", &width, &height);
	if (!game->img.wall)
	    error_exit("Failed to load wall.xpm");
	game->img.floor = mlx_xpm_file_to_image(game->mlx, "img/floor.xpm", &width, &height);
	if (!game->img.floor)
	    error_exit("Failed to load floor.xpm");
	game->img.player[0] = mlx_xpm_file_to_image(game->mlx, "img/player1.xpm", &width, &height);
	if (!game->img.player[0])
	    error_exit("Failed to load player1.xpm");
	game->img.player[1] = mlx_xpm_file_to_image(game->mlx, "img/player2.xpm", &width, &height);
	if (!game->img.player[1])
	    error_exit("Failed to load player2.xpm");
	game->img.fire = mlx_xpm_file_to_image(game->mlx, "img/fire.xpm", &width, &height);
	if (!game->img.fire)
	    error_exit("Failed to load fire.xpm");
	game->img.exit = mlx_xpm_file_to_image(game->mlx, "img/exit.xpm", &width, &height);
	if (!game->img.exit)
	    error_exit("Failed to load exit.xpm");
	game->img.collectible = mlx_xpm_file_to_image(game->mlx, "img/collectible.xpm", &width, &height);
	if (!game->img.collectible)
		error_exit("Failed to load collectible.xpm");
	game->img.enemy = mlx_xpm_file_to_image(game->mlx, "img/ganon_sit.xpm", &width, &height);
	if (!game->img.enemy)
	    error_exit("Failed to load enemy.xpm");
}
