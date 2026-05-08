#include "../include/cub3d.h"



int	close_window(t_data *data)
{

	free_exit(data, 0, "user closed window");
	exit(0);
	return (0);
}

int handle_expose(void *param)
{
    t_data *data;
    data = (t_data *)param;
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
    return (0);
}

int	main_loop(t_data *data)
{
	move(data);
	check_death(data);
	move_sprite(data);
	render_frame(data);
	return (0);
}

void    default_dir(char orientation, t_data *data)
{
    if (orientation == 'N')
    {
        data->dirX = 0;   data->dirY = -1;
        data->planeX = 0.66; data->planeY = 0;
    }
    else if (orientation == 'S')
    {
        data->dirX = 0;   data->dirY = 1;
        data->planeX = -0.66; data->planeY = 0;
    }
    else if (orientation == 'E')
    {
        data->dirX = 1;   data->dirY = 0;
        data->planeX = 0; data->planeY = 0.66;
    }
    else if (orientation == 'W')
    {
        data->dirX = -1;  data->dirY = 0;
        data->planeX = 0; data->planeY = -0.66;
    }
}

int	initialization(t_data *data)
{
	ft_bzero(data, sizeof(t_data));
	map_init(data);
	printf("map init!\n");
	// data->sprite.x = 1.5;
	// data->sprite.y = 1.5;
	data->mlx = mlx_init();
	printf("mlx init!\n");
	data->win = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
	printf("mlx new window ok!\n");
	data->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	printf("mlx new image ok!\n");
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->line_length, &data->endian);
	printf("initialization has done!\n");
	printf("posX: %f, posY: %f\n", data->posX, data->posY);
	return (0);
}

int main()
{
	t_data data;
	initialization(&data);
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);
	mlx_hook(data.win, 3, 1L<<1, key_release, &data);
	mlx_hook(data.win, 6, (1L << 6), mouse_move, &data);
	printf("mlx key mouse hook ok!\n");

	mlx_expose_hook(data.win, handle_expose, &data);
	printf("mlx expose hook ok!\n");

	mlx_hook(data.win, 17, 0, close_window, &data);
	printf("mlx close window hook ok!\n");

	load_textures(&data);
	printf("textures loaded!\n");

	mlx_loop_hook(data.mlx, main_loop, &data);
	printf("main loop started!\n");

	mlx_loop(data.mlx);
	printf("mlx_loop started!\n");
	free_exit(&data, 0, "propery game stoped");
	return (0);
}
