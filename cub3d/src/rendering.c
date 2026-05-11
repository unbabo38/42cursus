/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   rendering.c                                       :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: t.mura <t.mura@student.42tokyo.jp>        #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/06 16:46:21 by t.mura           #+#    #+#              */
/*   Updated: 2026/05/11 11:31:47 by t.mura          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	check_death(t_data *data)
{
	double	dist;

	dist = sqrt(pow(data->posX - data->sprite.x, 2)
			+ pow(data->posY - data->sprite.y, 2));
	if (dist < 0.4)
	{
		printf("\a");
		printf("KILLED BY BARREL!\n");
		exit(0);
	}
}

void	move_sprite(t_data *data)
{
	data->sprite.enemySpeed = 0.0035;
	data->sprite.buffer = 0.2;
	data->sprite.nextX = data->sprite.x;
	if (data->sprite.x < data->posX)
		data->sprite.nextX += data->sprite.enemySpeed;
	else
		data->sprite.nextX -= data->sprite.enemySpeed;
	if (data->map[(int) data->sprite.y][(int)(data->sprite.nextX + (data->sprite.nextX > data->sprite.x ? data->sprite.buffer : -data->sprite.buffer))] != '1' &&
			data->map[(int) data->sprite.y][(int)(data->sprite.nextX + (data->sprite.nextX > data->sprite.x ? data->sprite.buffer : -data->sprite.buffer))] != 'D')
	data->sprite.x = data->sprite.nextX;
	data->sprite.nextY = data->sprite.y;
	if (data->sprite.y < data->posY)
		data->sprite.nextY += data->sprite.enemySpeed;
	else
		data->sprite.nextY -= data->sprite.enemySpeed;
	if (data->map[(int)(data->sprite.nextY + (data->sprite.nextY > data->sprite.y ? data->sprite.buffer : -data->sprite.buffer))][(int) data->sprite.x] != '1' &&
			data->map[(int)(data->sprite.nextY + (data->sprite.nextY > data->sprite.y ? data->sprite.buffer : -data->sprite.buffer))][(int) data->sprite.x] != 'D')
	{
		data->sprite.y = data->sprite.nextY;
	}
}

void	render_sprites(t_data *data)
{
	double	spr_x;
	double	spr_y;
	double	invDet;
	double	transformX;
	double	transformY;
	int		spr_screen_x;
	int		spr_height;
	int		spr_width;
	int		draw_start_y;
	int		draw_end_y;
	int		draw_start_x;
	int		draw_end_x;
	int		texX;
	int		d;
	int		texY;
	int		color;

	spr_x = data->sprite.x - data->posX;
	spr_y = data->sprite.y - data->posY;
	invDet = 1.0 / (data->planeX * data->dirY - data->dirX * data->planeY);
	transformX = invDet * (data->dirY * spr_x - data->dirX * spr_y);
	transformY = invDet * (-data->planeY * spr_x + data->planeX * spr_y);
	spr_screen_x = (int)((SCREEN_WIDTH / 2) * (1 + transformX / transformY));
	spr_height = abs((int)(SCREEN_HEIGHT / transformY));
	spr_width = abs((int)(SCREEN_HEIGHT / transformY));
	draw_start_y = -spr_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start_y < 0) draw_start_y = 0;
		draw_end_y = spr_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end_y >= SCREEN_HEIGHT) draw_end_y = SCREEN_HEIGHT - 1;
		draw_start_x = -spr_width / 2 + spr_screen_x;
	if (draw_start_x < 0) draw_start_x = 0;
		draw_end_x = spr_width / 2 + spr_screen_x;
	if (draw_end_x >= SCREEN_WIDTH) draw_end_x = SCREEN_WIDTH - 1;
		for (int x = draw_start_x; x < draw_end_x; x++)
		{
			texX = (int)(256 * (x - (-spr_width / 2 + spr_screen_x)) * 64 / spr_width) / 256;
			if (transformY > 0 && transformY < data->z_buffer[x])
			{
				for (int y = draw_start_y; y < draw_end_y; y++)
				{
					d = y * 256 - SCREEN_HEIGHT * 128 + spr_height * 128;
					texY = ((d * 64) / spr_height) / 256;
					color = get_pixel_color(&data->tex[5], texX, texY);
					if ((color & 0x00FFFFFF) != 0)
						my_mlx_pixel_put(data, x, y, color);
				}
			}
		}
}

int	mouse_move(int x, int y, t_data *data)
{
	double	rotSpeed;
	double	oldDirX;
	double	oldPlaneX;

	(void) y;
	static int	last_x = -1;

	if (last_x == -1)
	{
		last_x = x;
		return (0);
	}
	rotSpeed = (x - last_x) * 0.01;
	oldDirX = data->dirX;
	data->dirX = data->dirX * cos(rotSpeed) - data->dirY * sin(rotSpeed);
	data->dirY = oldDirX * sin(rotSpeed) + data->dirY * cos(rotSpeed);
	oldPlaneX = data->planeX;
	data->planeX = data->planeX * cos(rotSpeed) - data->planeY * sin(rotSpeed);
	data->planeY = oldPlaneX * sin(rotSpeed) + data->planeY * cos(rotSpeed);
	last_x = x;
	return (0);
}

void	draw_square(t_data *data, int x, int y, int size, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			if (x + i >= 0 && x + i < SCREEN_WIDTH && y + j >= 0 && y + j < SCREEN_HEIGHT)
			{
				my_mlx_pixel_put(data, x + i, y + j, color);
			}
			j++;
		}
		i++;
	}
}

void	draw_minimap(t_data *data)
{
	int	size;
	int	color;
	int	x, y;

	size = 5;
	y = 0;
	while (y < data->map_height)
	{
		x = 0;
		while (x < data->map_width)
		{
			color = (data->map[y][x] == '1') ? 0xFFFFFF : 0x000000;
			draw_square(data, x * size + 20, y * size + 20, size, color);
			x++;
		}
		y++;
	}
	draw_square(data, data->posX * size + 20, data->posY * size + 20, 3, 0xFF0000);
}

unsigned int	get_pixel_color(t_img *img, int x, int y)
{
	char	*dst;

	if (x < 0 || x >= 64 || y < 0 || y >= 64)
		return (0);
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *) dst);
}

void	load_textures(t_data *data)
{
	char	*paths[6];
	int	w, h;

	paths[0] = data->texture.no_path;
	paths[1] = data->texture.so_path;
	paths[2] = data->texture.we_path;
	paths[3] = data->texture.ea_path;
	paths[4] = "textures/door.xpm";
	paths[5] = "textures/barrel.xpm";
	for (int i = 0; i < 6; i++)
	{
		data->tex[i].img = mlx_xpm_file_to_image(data->mlx, paths[i], &w, &h);
		if (!data->tex[i].img)
			free_exit(data, 1, "invalid texture file");
		data->tex[i].addr = mlx_get_data_addr(data->tex[i].img,
			&data->tex[i].bits_per_pixel,
		&data->tex[i].line_length,
		&data->tex[i].endian);
	}
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = (char *) data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *) dst = color;
}

void	put_sail_floor(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < SCREEN_WIDTH)
	{
		j = 0;
		while (j < SCREEN_HEIGHT)
		{
			if (j < SCREEN_HEIGHT / 2)
				my_mlx_pixel_put(data, i, j, data->texture.ceiling_color);
			else
				my_mlx_pixel_put(data, i, j, data->texture.floor_color);
			j++;
		}
		i++;
	}
}

void	set_dir(t_data *data, const int x)
{
	data->dda.cameraX = 2 * x / (double) SCREEN_WIDTH - 1;
	data->dda.rayDirX = data->dirX + data->planeX * data->dda.cameraX;
	data->dda.rayDirY = data->dirY + data->planeY * data->dda.cameraX;
	data->dda.deltaDistX = (data->dda.rayDirX == 0) ? 1e30 : fabs(1 / data->dda.rayDirX);
	data->dda.deltaDistY = (data->dda.rayDirY == 0) ? 1e30 : fabs(1 / data->dda.rayDirY);
	data->dda.mapX = (int) data->posX;
	data->dda.mapY = (int) data->posY;
}

void	describe_wall1(t_data *data)
{
	if (data->dda.side == 0)
		data->dda.perpWallDist = (data->dda.sideDistX - data->dda.deltaDistX);
	else
		data->dda.perpWallDist = (data->dda.sideDistY - data->dda.deltaDistY);
	data->dda.lineHeight = (int)(SCREEN_HEIGHT / data->dda.perpWallDist);
	data->dda.drawStart = -data->dda.lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (data->dda.drawStart < 0)
		data->dda.drawStart = 0;
	data->dda.drawEnd = data->dda.lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (data->dda.drawEnd >= SCREEN_HEIGHT)
		data->dda.drawEnd = SCREEN_HEIGHT - 1;
	if (data->dda.side == 0)
	{
		if (data->dda.rayDirX < 0) data->dda.tex_num = 0;
			else data->dda.tex_num = 1;
	}
	else
	{
		if (data->dda.rayDirY < 0) data->dda.tex_num = 2;
			else data->dda.tex_num = 3;
	}
	if (data->dda.hit == 2)
		data->dda.tex_num = 4;
}

void	describe_wall2(t_data *data)
{
	if (data->dda.side == 0)
		data->dda.wallX = data->posY + data->dda.perpWallDist * data->dda.rayDirY;
	else
		data->dda.wallX = data->posX + data->dda.perpWallDist * data->dda.rayDirX;
	data->dda.wallX -= floor(data->dda.wallX);
	data->dda.texX = (int)(data->dda.wallX * (double) 64);
	if (data->dda.side == 0 && data->dda.rayDirX > 0) data->dda.texX = 64 - data->dda.texX - 1;
		if (data->dda.side == 1 && data->dda.rayDirY < 0) data->dda.texX = 64 - data->dda.texX - 1;
		data->dda.step = 1.0 * 64 / data->dda.lineHeight;
	data->dda.texPos = (data->dda.drawStart - SCREEN_HEIGHT / 2 + data->dda.lineHeight / 2) * data->dda.step;
}

void	describe_wall3(t_data *data, const int x)
{
	int	y;
	int	color;

	y = data->dda.drawStart;
	while (y < data->dda.drawEnd)
	{
		data->dda.texY = (int) data->dda.texPos & (64 - 1);
		data->dda.texPos += data->dda.step;
		if (data->dda.texY < 0) data->dda.texY = 0;
			color = get_pixel_color(&data->tex[data->dda.tex_num], data->dda.texX, data->dda.texY);
		if (data->dda.side == 1) color = (color >> 1) & 8355711;
			my_mlx_pixel_put(data, x, y, color);
		y++;
	}
	data->z_buffer[x] = data->dda.perpWallDist;
}

void	dda(t_data *data)
{
	data->dda.hit = 0;
	while (data->dda.hit == 0)
	{
		if (data->dda.sideDistX < data->dda.sideDistY)
		{
			data->dda.sideDistX += data->dda.deltaDistX;
			data->dda.mapX += data->dda.stepX;
			data->dda.side = 0;
		}
		else
		{
			data->dda.sideDistY += data->dda.deltaDistY;
			data->dda.mapY += data->dda.stepY;
			data->dda.side = 1;
		}
		if (data->dda.mapX >= 0 && data->dda.mapX < data->map_width && data->dda.mapY >= 0 && data->dda.mapY < data->map_height)
		{
			if (data->map[data->dda.mapY][data->dda.mapX] == '1')
				data->dda.hit = 1;
			else if (data->map[data->dda.mapY][data->dda.mapX] == 'D')
				data->dda.hit = 2;
		}
		else
		{
			data->dda.hit = 1;
		}
	}
}

void	calc_dist_to_wall(t_data *data)
{
	if (data->dda.rayDirX < 0)
	{
		data->dda.stepX = -1;
		data->dda.sideDistX = (data->posX - data->dda.mapX) * data->dda.deltaDistX;
	}
	else
	{
		data->dda.stepX = 1;
		data->dda.sideDistX = (data->dda.mapX + 1.0 - data->posX) * data->dda.deltaDistX;
	}
	if (data->dda.rayDirY < 0)
	{
		data->dda.stepY = -1;
		data->dda.sideDistY = (data->posY - data->dda.mapY) * data->dda.deltaDistY;
	}
	else
	{
		data->dda.stepY = 1;
		data->dda.sideDistY = (data->dda.mapY + 1.0 - data->posY) * data->dda.deltaDistY;
	}
}

int	render_frame(t_data *data)
{
	int	x;

	put_sail_floor(data);
	x = 0;
	while (x < SCREEN_WIDTH)
	{
		set_dir(data, x);
		calc_dist_to_wall(data);
		dda(data);
		describe_wall1(data);
		describe_wall2(data);
		describe_wall3(data, x);
		x++;
	}
	draw_minimap(data);
	if (data->sprite.exist)
		render_sprites(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}
