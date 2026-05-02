#include "../include/cub3d.h"

int	main_loop(t_data *data)
{

	move(data);

	render_frame(data);

	return (0);
}

void	default_dir(char orientation, t_data *data)
{
	if (orientation == 'N')
	{
		data->dirX = -1;
		data->dirY = 0;
	}
	if (orientation == 'S')
	{
		data->dirX = 1;
		data->dirY = 0;
	}
	if (orientation == 'E')
	{
		data->dirX = 0;
		data->dirY = 1;
	}
	if (orientation == 'W')
	{
		data->dirX = 0;
		data->dirY = -1;
	}
}
void debug_make_map(t_data *data)
{
    int height = 18;
    int width = 30;

    data->map_height = height;
    data->map_width = width;

    // 1. 行方向のポインタ配列を確保
    data->map = malloc(sizeof(char *) * (height + 1));

    // 2. 各行を文字列として作成（昨日の「スペース広め」マップ）
    char *raw_map[] = {
        "111111111111111111111111111111",
        "100000000000000000000000000001",
        "100000000000000000000000000001",
        "100111111110000001111111100001",
        "100100000010000001000000100001",
        "100101110010000001001100100001",
        "100101010010000001001100100001",
        "100001010010000001000000100001",
        "100001110010000001111111100001",
        "100000000010000000000000000001",
        "100000000010000000000000000001",
        "1001111100100000000001111100001",
        "100100010010000000001000100001",
        "100100010011111111111000100001",
        "100100000000000000000000000001",
        "100111111111111111111111111101",
        "100000000000000000000000000001",
        "111111111111111111111111111111"
    };

    for (int i = 0; i < height; i++)
    {
        // 実際のメモリ空間を各行に割り当てる
        data->map[i] = malloc(sizeof(char) * (width + 1));
        // 中身をコピー
        for (int j = 0; j < width; j++)
            data->map[i][j] = raw_map[i][j];
        data->map[i][width] = '\0'; // 終端
    }
    data->map[height] = NULL;
}
int	initialization(t_data *data)
{

	ft_bzero(data, sizeof(t_data));
	//map_init(data);
	debug_make_map(data);
	data->posX = 3.0;
	data->posY = 3.0;
	//default_dir(orientation, data);
	data->dirX = -1.0;
	data->dirY = 0.0;
	data->planeX = 0;
	data->planeY = 0.66;
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
	data->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel, &data->line_length, &data->endian);

	return (0);
}

int main()
{
	t_data data;
	initialization(&data);
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);   // 押した時
	mlx_hook(data.win, 3, 1L<<1, key_release, &data); // 離した時
	mlx_loop_hook(data.mlx, main_loop, &data);
	mlx_loop(data.mlx);
	return (0);
}
