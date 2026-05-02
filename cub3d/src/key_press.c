#include "../include/cub3d.h"
// #define K_W 119
// #define K_ESC 27

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
	if (keycode == K_RIGHT)
		data->flg.key_right = 1;
	if (keycode == K_LEFT)
		data->flg.key_left = 1;
	if (keycode == K_ESC)
		data->flg.key_esc = 1;

	return 0;
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

	return 0;
}

int	move(t_data *data)
{
	double moveSpeed = 0.02;
	if (data->flg.key_w)
	{
		if (data->map[(int)(data->posX + data->dirX * moveSpeed)][(int)(data->posY)] !='1')
		{
			data->posX += data->dirX * moveSpeed;
		}
		if (data->map[(int)data->posX][(int)(data->posY + data->dirY * moveSpeed)] !='1')
			data->posY += data->dirY * moveSpeed;
	}
	if (data->flg.key_a)
	{
		if (data->map[(int)(data->posX - data->dirY * moveSpeed)][(int)(data->posY)] !='1')
			data->posX -= data->dirY * moveSpeed;
		if (data->map[(int)data->posX][(int)(data->posY + (data->dirX * moveSpeed))] !='1')
			data->posY += (data->dirX * moveSpeed);
	}
	if (data->flg.key_d)
	{
		if (data->map[(int)(data->posX + (data->dirY * moveSpeed))][(int)(data->posY)] !='1')
			data->posX += (data->dirY * moveSpeed);
		if (data->map[(int)data->posX][(int)(data->posY - data->dirX * moveSpeed)] !='1')
			data->posY -= data->dirX * moveSpeed;
	}
	if (data->flg.key_s)
	{
		if (data->map[(int)(data->posX - data->dirX * moveSpeed)][(int)(data->posY)] !='1')
			data->posX -= data->dirX * moveSpeed;
		if (data->map[(int)data->posX][(int)(data->posY - data->dirY * moveSpeed)] !='1')
			data->posY -= data->dirY * moveSpeed;
	}
	double rotSpeed = 0.005; // 回転速度
	if (data->flg.key_right) // 右回転
	{
		// dirを回転
		double oldDirX = data->dirX;
		data->dirX = data->dirX * cos(-rotSpeed) - data->dirY * sin(-rotSpeed);
		data->dirY = oldDirX * sin(-rotSpeed) + data->dirY * cos(-rotSpeed);
		// camera plane（視界の幅）も一緒に回転させないと視界が歪む
		double oldPlaneX = data->planeX;
		data->planeX = data->planeX * cos(-rotSpeed) - data->planeY * sin(-rotSpeed);
		data->planeY = oldPlaneX * sin(-rotSpeed) + data->planeY * cos(-rotSpeed);
	}
	if (data->flg.key_left) // 右回転
	{
		// dirを回転
		double oldDirX = data->dirX;
		data->dirX = data->dirX * cos(rotSpeed) - data->dirY * sin(rotSpeed);
		data->dirY = oldDirX * sin(rotSpeed) + data->dirY * cos(rotSpeed);
		// camera plane（視界の幅）も一緒に回転させないと視界が歪む
		double oldPlaneX = data->planeX;
		data->planeX = data->planeX * cos(rotSpeed) - data->planeY * sin(rotSpeed);
		data->planeY = oldPlaneX * sin(rotSpeed) + data->planeY * cos(rotSpeed);
	}
	if (data->flg.key_esc)
		exit(0);
	return (0);
}
