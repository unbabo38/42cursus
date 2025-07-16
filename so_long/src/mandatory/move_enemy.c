#include "../../include/so_long.h"

void	move_enemy(t_enemy *enemy, t_game *game)
{
	int h;
	int next_w;

	h = enemy->h;
	next_w= enemy->w + enemy->dir;
	if (game->map.data[h][next_w] == '1')
	{
		enemy->dir *= -1;
		return ;
	}
	if (game->elements.player_h == h && game->elements.player_w == next_w)
	{
		error_exit("GAME OVER\n");
		close_game(game);
	}
	game->map.data[h][enemy->w] = '0';
	game->map.data[h][next_w] = 'X';
	enemy->w = next_w;
}
