/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 10:01:42 by t.mura            #+#    #+#             */
/*   Updated: 2026/05/10 22:42:13 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../mlx/mlx.h"
# include "libft/libft.h"
# include "gnl/get_next_line.h"

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

# define K_W 119
# define K_A 97
# define K_S 115
# define K_D 100
# define K_E 101
# define K_LEFT 65361
# define K_RIGHT 65363
# define K_ESC 65307

# define PARSE_SUCCESS 0
# define PARSE_FAILED 2

# define EMPTY_LINE 3
# define IS_SPACE 1
# define IS_NOT_SPACE 0
# define LACK_OF_INFO 4
# define PATH_DUP 5
# define FAILED 6
# define IS_SPACE_OF_CRLF 1
# define FILENAME_WRONG 7

# define OK 0
# define ERROR -1
# define MAX_RGB 255
# define IS_TRUE 1
# define IS_FALSE -1
# define TRUE 0
# define FINISHED 1
# define NG 0

typedef struct s_dda
{
	double	cameraX;
	double	rayDirX;
	double	rayDirY;
	double	deltaDistX;
	double	deltaDistY;
	int		mapX;
	int		mapY;
	double	sideDistX;
	double	sideDistY;
	int		stepX;
	int		stepY;
	int		side;
	int		hit;
	double	perpWallDist;
	int		lineHeight;
	int		drawStart;
	int		drawEnd;
	int		tex_num;
	double	wallX;
	int		texX;
	int		texY;
	double	step;
	double	texPos;
}	t_dda;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_flg
{
	int	key_w;
	int	key_s;
	int	key_a;
	int	key_d;
	int	key_left;
	int	key_right;
	int	key_esc;
}	t_flg;

typedef struct s_texture
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_color;
	int		ceiling_color;
}	t_texture;

typedef struct s_sprite
{
	double	x;
	// マップ上の座標
	double	y;
	int		tex_id;
	// どのテクスチャを使うか
	int	exist;
}	t_sprite;

typedef struct s_map_info
{
	char	*line;
	int		in_map;
	int		parse_result;
	int		tmp;
	int		is_empty;
	int		map_finished;
	int		tmp_width;

}	t_map_info;

typedef struct s_pointer
{
	int	h;
	int	w;
	int	cnt;
}	t_pointer;

typedef struct s_cub3d
{
	char	*filename;
	void	*mlx;
	void	*win;
	void	*img;
	void	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	char	**map;
	char 	**copy_map;
	int		map_height;
	int		map_width;
	double	posX, posY;
	double	dirX, dirY;
	double	planeX, planeY;
	double	dx, dy;

	t_flg		flg;
	t_texture	texture;
	t_img		tex[6];
	double		z_buffer[SCREEN_WIDTH];
	t_sprite	sprite;
	t_dda		dda;
	t_map_info	map_info;
	int			fd;
	int			file_rows;
	int			i;
	int			idx;
	char 		*trimmed;
	t_pointer	pointer;
}	t_data;

int	render_frame(t_data *data);
int	key_press(int keycode, t_data *data);
int	key_release(int keycode, t_data *data);
int	move(t_data *data);
int	main_loop(t_data *data);
int	initialization(t_data *data);

int		map_init(t_data *data);
void	copy_line(char *dst, char *tmp, t_data *data);
void	make_map(int fd, t_data *data);

void			default_dir(char orientation, t_data *data);
int				check_characters(t_data *data);
void			check_wall(t_data *data, char **map, int h, int w);
void			free_stab(char **tab);
int				parse_rgb(char *str);
void			load_textures(t_data *data);
int				parse_config_line(char *line, t_data *data);
void			my_mlx_pixel_put(t_data *data, int x, int y, int color);
void			draw_square(t_data *data, int x, int y, int size, int color);
int				mouse_move(int x, int y, t_data *data);
unsigned int	get_pixel_color(t_img *img, int x, int y);
void			move_sprite(t_data *data);
void			check_death(t_data *data);
void			dda(t_data *data);
void			free_exit(t_data *data, int status, char *error_msg);
void			free_map(t_data *data);
int				ft_is_space(char *character);
int				is_space(char c);
void			check_and_get_map_info(t_data *data);
void			free_textures(t_data *data);
int				is_config_info(t_data *data);
void			copy_line_to_map(t_data *data);

# define mapWidth 24
# define mapHeight 24

extern int	worldMap[mapWidth][mapHeight];

#endif
