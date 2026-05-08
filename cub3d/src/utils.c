/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 23:21:45 by tmura             #+#    #+#             */
/*   Updated: 2026/05/06 23:21:45 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	free_map(t_data *data)
{
	int	i;

	if (data->map)
	{
		i = 0;
		while (i < data->map_height)
		{
			free(data->map[i]);
			i++;
		}
		free(data->map);
		data->map = NULL;
	}
}



int	free_exit(t_data *data, int status, char *error_msg)
{
	int	i;

	// 1. エラーメッセージの出力（statusが0以外の場合）
	if (status != 0 && error_msg)
	{
		write(2, "Error\n", 6);
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	if (data->texture.no_path)
		free(data->texture.no_path);
	if (data->texture.so_path)
		free(data->texture.so_path);
	if (data->texture.we_path)
		free(data->texture.we_path);
	if (data->texture.ea_path)
		free(data->texture.ea_path);
	if (data->map)
		free_map(data);
	// 3. テクスチャ画像の解放 (t_img tex[6] のループ)
	i = 0;
	while (i < 6)
	{
		if (data->tex[i].img)
			mlx_destroy_image(data->mlx, data->tex[i].img);
		i++;
	}

	// 4. メインフレームバッファ(img)の解放
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);

	// 5. ウィンドウの破壊
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);

	// 6. MLXポインタの解放 (環境によっては必要ないが、Linux版等では推奨)
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx); // これが重要！
		free(data->mlx);               // mlx_initで確保されたポインタ自体をfree
	}

	// 7. 終了
	exit(status);
	return (0);
}
