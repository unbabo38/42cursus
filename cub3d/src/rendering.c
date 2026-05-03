#include "../include/cub3d.h"



void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	// 計算結果を一旦 char * の変数に入れる（これならエラーが出ない）
	dst = (char *)data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));

	// その住所に 4バイトの色を書き込む
	*(unsigned int*)dst = color;
}


int	render_frame(t_data *data)
{
	//printf("DEBUG: addr = %p, width = %d, height = %d\n", data->addr, SCREEN_WIDTH, SCREEN_HEIGHT);
	int i = 0;
	while (i < SCREEN_WIDTH) {
		int j = 0;
		while (j < SCREEN_HEIGHT) {
			if (j < SCREEN_HEIGHT / 2)
				my_mlx_pixel_put(data, i, j, 0x00333333); // 天井（グレー）
			else
				my_mlx_pixel_put(data, i, j, 0x00777777); // 床（明るいグレー）
			j++;
		}
		i++;
	}
	//data->posX = 12.0, data->posY = 12.0;
	//data->planeX = 0, data->planeY = 0.66;

	int	x = 0;
	while (x < SCREEN_WIDTH)
	{

		double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;

		// dirXという方向を向いてて、視野がplaneXほどある
		// 視野のcameraXという地点へrayDirXは向いている
		double rayDirX = data->dirX + data->planeX * cameraX;
		double rayDirY = data->dirY + data->planeY * cameraX;


		double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

		int 	mapX = (int)data->posX;
		int 	mapY = (int)data->posY;

		double 	sideDistX, sideDistY;
		int 	stepX, stepY;

		//　初期位置から、最初の壁に当たるまでの距離の計算
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (data->posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - data->posX) * deltaDistX;
		}

		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (data->posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - data->posY) * deltaDistY;
		}

		int hit = 0;
		int side;
		// DDA本体
		// if (mapX < 0 || mapY < 0 || mapX >= 7 || mapY >= 5) {
		// 	printf("OUT OF BOUNDS: x:%d, y:%d\n", mapX, mapY);
		// 	return (0);
		// }
		while(hit == 0)
		{
			//printf("Checking map[%d][%d]\n", mapY, mapX);
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (mapX >= 0 && mapX < data->map_width && mapY >= 0 && mapY < data->map_height) {
				if (data->map[mapX][mapY] == '1') hit = 1;
			} else {
				hit = 1; // 範囲外に出たらループを抜ける
			}
		}

		// この時点でhitしている、ここから描画処理
		double perpWallDist;
		if (side == 0) 	perpWallDist = (sideDistX - deltaDistX);
		else			perpWallDist = (sideDistY - deltaDistY);

		int	lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT /2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

		int y = drawStart;
		while (y < drawEnd) {
			int color = (side == 1) ? 0x00AA0000 : 0x00FF0000; // 横面と縦面で色を変えると立体感が出る！
			my_mlx_pixel_put(data, x, y, color);
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return 0;
}
