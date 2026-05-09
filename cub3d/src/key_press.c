#include "../include/cub3d.h"

void open_close_door(t_data *data)
{
    int targetX = (int)(data->posX + data->dirX * 1.2);
    int targetY = (int)(data->posY + data->dirY * 1.2);

    if (targetX < 0 || targetX >= data->map_width || targetY < 0 || targetY >= data->map_height)
        return ;

    if (data->map[targetY][targetX] == 'D')
        data->map[targetY][targetX] = 'O';
    else if (data->map[targetY][targetX] == 'O')
    {
        if ((int)data->posX != targetX || (int)data->posY != targetY)
            data->map[targetY][targetX] = 'D';
    }
}

char *skip_whitespace(char *line)
{
	if (!line)
        return (NULL);
    while (*line == ' ' || (*line >= 9 && *line <= 13))
        line++;
    return (line);
}

void debug_binary(void *ptr, size_t size)
{
    unsigned char *p = (unsigned char *)ptr;
    size_t i = 0;

    printf("Addr: %p | Data: ", ptr);
    while (i < size)
    {
        // 16進数で表示（02x は、1桁でも 05 のように 2桁で表示する設定）
        printf("%02x ", p[i]);
        i++;
    }
    printf("| ");

    // おまけ：文字として読める場合は文字も表示（ASCII確認）
    i = 0;
    while (i < size)
    {
        if (p[i] >= 32 && p[i] <= 126)
            printf("%c", p[i]);
        else
            printf("."); // 読めない文字はドット
        i++;
    }
    printf("\n");
}

int parse_rgb(char *str)
{
    char **colors;
    int r, g, b;

    // 1. カンマで分割
    colors = ft_split(str, ',');
    if (!colors || !colors[0] || !colors[1] || !colors[2])
		return (-1);
	if (ft_is_space(colors[0]) || ft_is_space(colors[1]) || ft_is_space(colors[2]))
		return (-1);
    // 2. 数値に変換
    r = ft_atoi(colors[0]);
    g = ft_atoi(colors[1]);
    b = ft_atoi(colors[2]);

	printf("b=%d\n", b);


    // 3. 範囲チェック（0-255）
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
	    free_tab(colors);
	    return (-1); // エラー
	}

    // メモリ解放（ft_splitの結果をfreeするのを忘れずに！）
    free_tab(colors);

    // 4. ビットシフトで1つのintにまとめる
    // Redを16bit、Greenを8bit左にずらして重ねる
    return (r << 16 | g << 8 | b);
}

int parse_config_line(char *line, t_data *data)
{
    char *trimmed = line;
    while (*trimmed && (*trimmed == ' ' || (*trimmed >= 9 && *trimmed <= 13)))
        trimmed++;
    if (*trimmed == '\0' || *trimmed == '\n')
        return (1);
    if (ft_strncmp(trimmed, "NO", 2) == 0)
	{
		if (data->texture.no_path)
			return (2);
        data->texture.no_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
	}
    else if (ft_strncmp(trimmed, "SO", 2) == 0)
	{
		if (data->texture.so_path)
			return (2);
        data->texture.so_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
	}
    else if (ft_strncmp(trimmed, "WE", 2) == 0)
	{
		if (data->texture.we_path)
			return (2);
        data->texture.we_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
	}
    else if (ft_strncmp(trimmed, "EA", 2) == 0)
	{
		if (data->texture.ea_path)
			return (2);
        data->texture.ea_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
	}
	else if (ft_strncmp(trimmed, "F", 1) == 0)
	{
		if (data->texture.floor_color != 0)
			return 2;
		printf("floor_color=%d\n", data->texture.floor_color);
		data->texture.floor_color = parse_rgb(trimmed + 2);
		if (data->texture.floor_color == -1)
		{
			printf("parse_failed_floor_color\n");
			return 2;
		}
	}

    else if (ft_strncmp(trimmed, "C", 1) == 0)
	{
		if (data->texture.ceiling_color != 0)
			return 2;
        data->texture.ceiling_color = parse_rgb(trimmed + 2);
		printf("ceiling_color=%d\n", data->texture.ceiling_color);

		if (data->texture.ceiling_color == -1)
		{
			printf("parse_failed_ceiling_color\n");
			return 2;
		}
	}
    else
    {
        // ここがポイント！ '1' や '0' が見つかったら「ここはマップだ」と判定
        if (*trimmed == '1' || *trimmed == '0')
            return (0);
		printf("inmap?\n");
        return (1); // それ以外の不明なゴミ行は無視
    }
	printf("last return?\n");
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
	if (keycode == K_E) // K_E はお使いのヘッダーで定義した E のキーコード
	{
		open_close_door(data);
	}
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

void	calc_move_amount(t_data *data)
{
	double speed = 0.05;
	data->dx = 0;
	data->dy = 0;

    if (data->flg.key_w) {
        data->dx += data->dirX * speed;
        data->dy += data->dirY * speed;
    }
    if (data->flg.key_s) {
        data->dx -= data->dirX * speed;
        data->dy -= data->dirY * speed;
    }
    if (data->flg.key_a) {
        data->dx += data->dirY * speed;
        data->dy -= data->dirX * speed;
    }
    if (data->flg.key_d) {
        data->dx -= data->dirY * speed;
        data->dy += data->dirX * speed;
    }
}

void	check_wall_exit(t_data *data)
{
	double marginX = (data->dx >= 0) ? 0.2 : -0.2;
    double marginY = (data->dy >= 0) ? 0.2 : -0.2;

    if (data->map[(int)data->posY][(int)(data->posX + data->dx + marginX)] != '1'
		&& data->map[(int)data->posY][(int)(data->posX + data->dx + marginX)] != 'D')
        data->posX += data->dx;
    if (data->map[(int)(data->posY + data->dy + marginY)][(int)data->posX] != '1'
		&& data->map[(int)(data->posY + data->dy + marginY)][(int)data->posX] != 'D')
	{
        data->posY += data->dy;
	}
}

void	rotate(t_data *data)
{
	double rotSpeed = 0.005; // 回転速度
	if (data->flg.key_right) // 右回転
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
	if (data->flg.key_left) // 右回転
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
