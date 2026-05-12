/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:10:09 by tmura             #+#    #+#             */
/*   Updated: 2026/05/12 16:10:09 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

void	check_wall(t_data *data, char **map, int h, int w)
{
	if (h < 0 || h >= data->map_height)
		free_exit(data, FAILED, "Map is open (out of height)!");
	if (w < 0 || !map[h] || w >= (int)ft_strlen(map[h]))
		free_exit(data, FAILED, "Map is open (out of width)!");
	if (!map[h])
		free_exit(data, FAILED, "map does not exist!");
	if (is_space(map[h][w]))
		free_exit(data, FAILED, "space in the map!");
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
	if (check_characters(data) != 0)
	{
		free_exit(data, FAILED, "check_characters failed\n");
		return (1);
	}
	data->cmap = copy_map(data);
	check_wall(data, data->cmap, (int) data->pos_y, (int) data->pos_x);
	free_stab(data->cmap);
	return (0);
}
