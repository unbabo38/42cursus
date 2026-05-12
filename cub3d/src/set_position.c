/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_sprite.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:14:32 by tmura             #+#    #+#             */
/*   Updated: 2026/05/12 16:14:32 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	set_player_position(t_data *data)
{
	data->pos_x = data->pointer.w + 0.5;
	data->pos_y = data->pointer.h + 0.5;
	default_dir(data->map[data->pointer.h][data->pointer.w], data);
	data->map[data->pointer.h][data->pointer.w] = '0';
	data->pointer.cnt++;
}

void	set_sprite_position(t_data *data)
{
	data->sprite.x = data->pointer.w + 0.5;
	data->sprite.y = data->pointer.h + 0.5;
	data->map[data->pointer.h][data->pointer.w] = '0';
	data->sprite.exist = 1;
}
