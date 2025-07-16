#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>


# include "../libft/libft.h"
# include "../gnl/get_next_line.h"

// タイルサイズ（px）
# define TILE_SIZE 32

// キーコード（Linuxの場合）
# define KEY_ESC 65307
# define KEY_W   119
# define KEY_A   97
# define KEY_S   115
# define KEY_D   100

// エラー出力
# define ERR_MSG_INVALID_EXT   "Error: Invalid file extension. Use .ber"
# define ERR_MSG_OPEN_FAIL     "Error: Cannot open file"
# define ERR_MSG_INVALID_MAP   "Error: Invalid map"

# define VALID_PATH 1
# define INVALID_PATH 0
# define VALID_MAP 1
# define INVALID_MAP 0



typedef struct s_pos
{
	int	h;
	int	w;
}	t_pos;

typedef struct s_move
{
	int	new_h;
	int	new_w;
	int	old_h;
	int	old_w;
}	t_move;

typedef struct s_map
{
	char	**data;
	int		width;
	int		height;
	int		player_count;
	int		exit_count;
	int		collectible_count;
}	t_map;

typedef struct s_img
{
	void	*wall;
	void	*floor;
	void	*player[2];
	void	*exit;
	void	*collectible;
	void	*enemy;
	void	*fire;
}	t_img;

typedef struct s_elements {
	int player_count;
	int exit_count;
	int collectible_count;
	int player_h;
	int player_w;
}	t_elements;

typedef struct s_enemy {
	int h;
	int w;
	int dir;
} t_enemy;

typedef struct s_game
{
	void	*mlx;
	void	*win;

	t_map		map;
	t_img		img;
	t_elements 	elements;
	t_enemy		enemy;

	int		move_count;
	int		collected;
	int		anim_frame;
	int		real_frame;
}	t_game;



// ------------ init / exit ------------
void	exit_with_error(const char *msg);
void	free_map(char **map);

// ------------ map ------------
void	read_line_to_map(t_game *game, char *filename);
char	*line_to_lines(const int fd);
void	check_wall(const t_game *game);
void	check_left_walls(char **map, int height);
void	check_right_walls(char **map, int width, int height);
int		has_ber_extension(const char *filename);
void	check_shape(const t_game *game);

// ------------ render ------------
void	init_mlx(t_game *game);

// ------------ input ------------
void	load_images(t_game *game);



// ------------ utils ------------
int		ft_strcmp(const char *s1, const char *s2);
int		open_file(const char *filename);



void	split_error();
void	read_line_error();
void	open_error();
void	invalid_map();
void	shape_error();
void	error_exit(const char *error_massages);


void	free_map(char **map);
void	free_all(const t_game *game);


void	check_elements(t_game *game);

void	draw_fore_ground_tile(const t_game *game,const char tile, const int w, const int h);
void	draw_back_ground_tile(const t_game *game,const char tile, const int w, const int h);

void	draw_map(const t_game *game);

void	move_player(t_game *game, int dx, int dy);
void	move_enemy(t_enemy *enemy, t_game *game);
int		update_frame(t_game *game);


int		handle_keypress(int keycode, t_game *game);



int		close_game(t_game *game);
int		is_valid_path(char **map, int h, int w);
int		is_valid_map(t_game *game);

void	move_forward(t_game *game, t_move *move);
int		action(char next, t_game *game, t_move *move);


#endif
