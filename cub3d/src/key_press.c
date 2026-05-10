/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   key_press.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: t.mura <t.mura@student.42tokyo.jp>        #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/09 19:42:30 by t.mura           #+#    #+#              */
/*   Updated: 2026/05/10 23:01:41 by t.mura          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	open_close_door(t_data *data)
{
	int	target_x;
	int	target_y;

	target_x = (int)(data->posX + data->dirX * 1.2);
	target_y = (int)(data->posY + data->dirY * 1.2);
	if (target_x < 0 || target_x >= data->map_width
		|| target_y < 0 || target_y >= data->map_height)
		return ;
	if (data->map[target_y][target_x] == 'D')
		data->map[target_y][target_x] = 'O';
	else if (data->map[target_y][target_x] == 'O')
	{
		if ((int) data->posX != target_x || (int) data->posY != target_y)
			data->map[target_y][target_x] = 'D';
	}
}

int	parse_rgb(char *str)
{
	char	**colors;
	int		r;
	int		g;
	int		b;

	colors = ft_split(str, ',');
	if (!colors || !colors[0] || !colors[1] || !colors[2])
		return (ERROR);
	if (ft_is_space(colors[0]) || ft_is_space(colors[1])
		|| ft_is_space(colors[2]))
	{
		free_stab(colors);
		return (ERROR);
	}
	r = ft_atoi(colors[0]);
	g = ft_atoi(colors[1]);
	b = ft_atoi(colors[2]);
	if (r < 0 || r > MAX_RGB || g < 0 || g > MAX_RGB || b < 0 || b > MAX_RGB)
	{
		free_stab(colors);
		return (ERROR);
	}
	free_stab(colors);
	return (r << 16 | g << 8 | b);
}

void	set_north_path(t_data *data, char *trimmed)
{
	if (data->texture.no_path)
		free_exit(data, PATH_DUP, data->texture.no_path);
	data->texture.no_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
}

void	set_south_path(t_data *data, char *trimmed)
{
	if (data->texture.so_path)
		free_exit(data, PATH_DUP, data->texture.so_path);
	data->texture.so_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
}

void	set_west_path(t_data *data, char *trimmed)
{
	if (data->texture.we_path)
		free_exit(data, PATH_DUP, data->texture.we_path);
	data->texture.we_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
}

void	set_east_path(t_data *data, char *trimmed)
{
	if (data->texture.ea_path)
		free_exit(data, PATH_DUP, data->texture.ea_path);
	data->texture.ea_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
}

void	set_floor_color(t_data *data, char *trimmed)
{
	if (data->texture.floor_color != 0)
		free_exit(data, PATH_DUP, "FLOOR");
	data->texture.floor_color = parse_rgb(trimmed + 2);
	if (data->texture.floor_color == -1)
		free_exit(data, FAILED, data->map_info.line);
}

void	set_ceiling_color(t_data *data, char *trimmed)
{
	if (data->texture.ceiling_color != 0)
		free_exit(data, PATH_DUP, "CEILING");
	data->texture.ceiling_color = parse_rgb(trimmed + 2);
	if (data->texture.ceiling_color == -1)
		free_exit(data, PARSE_FAILED, data->map_info.line);
}

int	parse_config_line(char *line, t_data *data)
{
	data->trimmed = line;
	while (*data->trimmed && (*data->trimmed == ' '
			|| (*data->trimmed >= 9 && *data->trimmed <= 13)))
		data->trimmed++;
	if (*data->trimmed == '\0' || *data->trimmed == '\n')
		return (1);
	if (ft_strncmp(data->trimmed, "NO ", 3) == 0)
		set_north_path(data, data->trimmed);
	else if (ft_strncmp(data->trimmed, "SO ", 3) == 0)
		set_south_path(data, data->trimmed);
	else if (ft_strncmp(data->trimmed, "WE ", 3) == 0)
		set_west_path(data, data->trimmed);
	else if (ft_strncmp(data->trimmed, "EA ", 3) == 0)
		set_east_path(data, data->trimmed);
	else if (ft_strncmp(data->trimmed, "F ", 2) == 0)
		set_floor_color(data, data->trimmed);
	else if (ft_strncmp(data->trimmed, "C ", 2) == 0)
		set_ceiling_color(data, data->trimmed);
	else
	{
		if (*data->trimmed == '1' || *data->trimmed == '0')
			return (0);
		return (1);
	}
	return (1);
}

int	key_press(int keycode, t_data *data)
{
	if (keycode == K_W)
		data->flg.key_w = 1;
	if (keycode == K_A)
		data->flg.key_a = 1;
	if (keycode == K_D)
		data->flg.key_d = 1;
	if (keycode == K_S)
		data->flg.key_s = 1;
	if (keycode == K_E)
	{
		open_close_door(data);
	}
	if (keycode == K_RIGHT)
		data->flg.key_right = 1;
	if (keycode == K_LEFT)
		data->flg.key_left = 1;
	if (keycode == K_ESC)
		data->flg.key_esc = 1;
	return (0);
}

int	key_release(int keycode, t_data *data)
{
	if (keycode == K_W)
		data->flg.key_w = 0;
	if (keycode == K_A)
		data->flg.key_a = 0;
	if (keycode == K_D)
		data->flg.key_d = 0;
	if (keycode == K_S)
		data->flg.key_s = 0;
	if (keycode == K_RIGHT)
		data->flg.key_right = 0;
	if (keycode == K_LEFT)
		data->flg.key_left = 0;
	if (keycode == K_ESC)
		data->flg.key_esc = 0;
	return (0);
}

void	calc_move_amount(t_data *data)
{
	double	speed;

	speed = 0.05;
	data->dx = 0;
	data->dy = 0;
	if (data->flg.key_w)
	{
		data->dx += data->dirX * speed;
		data->dy += data->dirY * speed;
	}
	if (data->flg.key_s)
	{
		data->dx -= data->dirX * speed;
		data->dy -= data->dirY * speed;
	}
	if (data->flg.key_a)
	{
		data->dx += data->dirY * speed;
		data->dy -= data->dirX * speed;
	}
	if (data->flg.key_d)
	{
		data->dx -= data->dirY * speed;
		data->dy += data->dirX * speed;
	}
}

void	check_wall_exit(t_data *data)
{
	double	margin_x;
	double	margin_y;

	if (data->dx >= 0)
		margin_x = 0.2;
	else
		margin_x = -0.2;
	if (data->dy >= 0)
		margin_y = 0.2;
	else
		margin_y = -0.2;
	if (data->map[(int)data->posY][(int)(data->posX + data->dx + margin_x)]
		!= '1'
		&& data->map[(int)data->posY][(int)(data->posX + data->dx + margin_x)]
		!= 'D')
		data->posX += data->dx;
	if (data->map[(int)(data->posY + data->dy + margin_y)][(int) data->posX]
		!= '1'
		&& data->map[(int)(data->posY + data->dy + margin_y)][(int) data->posX]
		!= 'D')
		data->posY += data->dy;
}

void	rotate(t_data *data)
{
	double	rot_speed;
	double	old_dir_x;
	double	old_planet_x;

	rot_speed = 0.005;
	if (data->flg.key_right)
	{
		old_dir_x = data->dirX;
		data->dirX = data->dirX * cos(rot_speed) - data->dirY * sin(rot_speed);
		data->dirY = old_dir_x * sin(rot_speed) + data->dirY * cos(rot_speed);
		old_planet_x = data->planeX;
		data->planeX = data->planeX * cos(rot_speed)
		- data->planeY * sin(rot_speed);
		data->planeY = old_planet_x * sin(rot_speed)
		+ data->planeY * cos(rot_speed);
	}
	if (data->flg.key_left)
	{
		old_dir_x = data->dirX;
		data->dirX = data->dirX * cos(-rot_speed) - data->dirY * sin(-rot_speed);
		data->dirY = old_dir_x * sin(-rot_speed) + data->dirY * cos(-rot_speed);
		old_planet_x = data->planeX;
		data->planeX = data->planeX * cos(-rot_speed) - data->planeY * sin(-rot_speed);
		data->planeY = old_planet_x * sin(-rot_speed) + data->planeY * cos(-rot_speed);
	}
}

int	move(t_data *data)
{
	calc_move_amount(data);
	check_wall_exit(data);
	rotate(data);
	if (data->flg.key_esc)
	{
		free_exit(data, 0, "esc pushed\n");
	}
	return (0);
}
