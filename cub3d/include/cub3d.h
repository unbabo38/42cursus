#ifndef CUB3D_H
#define CUB3D_H

#include "../mlx/mlx.h"
#include "libft/libft.h"
#include "gnl/get_next_line.h"


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define K_W 119
#define K_A 97
#define K_S 115
#define K_D 100
#define K_E 101
#define K_LEFT 65361
#define K_RIGHT 65363
#define K_ESC 65307

typedef struct s_img {
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
    int     width;
    int     height;
} t_img;

typedef struct s_flg {
    int key_w;
    int key_s;
    int key_a;
    int key_d;
    int key_left;
    int key_right;
	int	key_esc;
} t_flg;

typedef struct s_texture {
	char    *no_path;
    char    *so_path;
    char    *we_path;
    char    *ea_path;
    int     floor_color;
    int     ceiling_color;
} t_texture;

typedef struct s_sprite
{
    double x;      // マップ上の座標
    double y;
    int    tex_id; // どのテクスチャを使うか
} t_sprite;

typedef struct cub3d {
	void 	*mlx;
	void 	*win;
	void 	*img;
	void 	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	char 	**map;
	int		map_height;
	int		map_width;
	double 	posX, posY;
	double 	dirX, dirY;
	double 	planeX, planeY;
	t_flg 	flg;
	t_texture texture;
	t_img   tex[6];
	double z_buffer[SCREEN_WIDTH];
	t_sprite sprite;
} t_data;



// typedef struct input {
// 	int
// } t_input;
int	render_frame(t_data *data);
int	key_press(int keycode, t_data *data);
int	key_release(int keycode, t_data *data);
int	move(t_data *data);
int	main_loop(t_data *data);
int	initialization(t_data *data);

int	map_init(t_data *data);
void fill_space(char *dst, char *tmp, t_data *data);
void make_map(int fd, t_data *data);

void	default_dir(char orientation, t_data *data);
int	check_characters(t_data *data);
int	check_wall(t_data *data, char **map, int h, int w);
void free_tab(char **tab);
int parse_rgb(char *str);
char *skip_whitespace(char *line);
void load_textures(t_data *data);
int parse_config_line(char *line, t_data *data);
void	my_mlx_pixel_put(t_data *data, int x, int y, int color);
void    draw_square(t_data *data, int x, int y, int size, int color);
int	mouse_move(int x, int y, t_data *data);
unsigned int get_pixel_color(t_img *img, int x, int y);
void move_sprite(t_data *data);
void check_death(t_data *data);

#define mapWidth 24
#define mapHeight 24


extern int worldMap[mapWidth][mapHeight];



#endif
