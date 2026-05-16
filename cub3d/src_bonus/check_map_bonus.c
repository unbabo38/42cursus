/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 17:03:15 by tmura             #+#    #+#             */
/*   Updated: 2026/05/16 17:03:15 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d_bonus.h"

void	free_stab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_stab_exit(t_data *data, char **map, int num, char *msg)
{
	free_stab(map);
	free_exit(data, num, msg);
}

void	check_wall(t_data *data, char **map, int h, int w)
{
	printf("\nmaxh=%d, maxw=%d", data->map_height, data->map_width);
	printf("\nh=%d, w=%d", h, w);
	if (h < 0 || h >= data->map_height)
		free_stab_exit(data, map, FAILED, "Map is open");
	if (!map[h])
		free_stab_exit(data, map, FAILED, "Map does not exitst!");
	data->map_info.tmp_width = (int)ft_strlen(map[h]);
	if (w < 0 || !map[h] || w >= data->map_info.tmp_width)
		free_stab_exit(data, map, FAILED, "Map is open!");
	if (is_space(map[h][w]))
		free_stab_exit(data, map, FAILED, "space in the map!");
	if (!(map[h][w] == 'v' || map[h][w] == '1'
		|| map[h][w] == 'D' || map[h][w] == '0'))
		free_stab_exit(data, map, FAILED, "Unreachable area!");
	if (map[h][w] == '1' || map[h][w] == 'v')
		return ;
	map[h][w] = 'v';
	check_wall(data, map, h + 1, w);
	check_wall(data, map, h - 1, w);
	check_wall(data, map, h, w + 1);
	check_wall(data, map, h, w - 1);
}

int	check_characters(t_data *data)
{
	char	position;

	while (data->pointer.h < data->map_height)
	{
		data->pointer.w = 0;
		while (data->pointer.w < data->map_width)
		{
			position = data->map[data->pointer.h][data->pointer.w];
			if (position == 'N' || position == 'S'
				|| position == 'W' || position == 'E')
				set_player_position(data);
			if (position == 'K')
				set_sprite_position(data);
			data->pointer.w++;
		}
		data->pointer.h++;
	}
	if (data->pointer.cnt != 1)
	{
		return (1);
	}
	else
		return (0);
}

int	check_character_and_wall(t_data *data)
{
	int	x;
	int	y;

	if (check_characters(data) != 0)
	{
		free_exit(data, FAILED, "check_characters failed!");
		return (1);
	}
	data->cmap = copy_map(data);
	x = 0;
	y = 0;
	while (y < data->map_height)
	{
		while (x < (int)ft_strlen(data->map[y]))
		{
			if (data->map[y][x] != '1')
				check_wall(data, data->cmap, y, x);
			x++;
		}
		y++;
	}
	free_stab(data->cmap);
	return (0);
}
