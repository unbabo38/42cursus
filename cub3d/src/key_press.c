#include "../include/cub3d.h"
// #define K_W 119
// #define K_ESC 27

void open_close_door(t_data *data)
{
    // プレイヤーの少し前（1.2マス先）の座標を計算
    int targetX = (int)(data->posX + data->dirX * 1.2);
    int targetY = (int)(data->posY + data->dirY * 1.2);

    // 境界チェック（マップ外アクセス防止）
    if (targetX < 0 || targetX >= data->map_width || targetY < 0 || targetY >= data->map_height)
        return ;

    if (data->map[targetY][targetX] == 'D') // 閉じたドア
    {
        data->map[targetY][targetX] = 'O'; // 'O'pen に変更
    }
    else if (data->map[targetY][targetX] == 'O') // 開いたドア
    {
        // プレイヤー自身がドアの真ん中にいないかチェック（挟まり防止）
        if ((int)data->posX != targetX || (int)data->posY != targetY)
            data->map[targetY][targetX] = 'D'; // 'D'oor に戻す
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

int parse_rgb(char *str)
{
    char **colors;
    int r, g, b;

    // 1. カンマで分割
    colors = ft_split(str, ',');
    if (!colors || !colors[0] || !colors[1] || !colors[2])
        return (-1); // エラー処理

    // 2. 数値に変換
    r = ft_atoi(colors[0]);
    g = ft_atoi(colors[1]);
    b = ft_atoi(colors[2]);

    // 3. 範囲チェック（0-255）
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return (-1); // エラー

    // メモリ解放（ft_splitの結果をfreeするのを忘れずに！）
    free_tab(colors);

    // 4. ビットシフトで1つのintにまとめる
    // Redを16bit、Greenを8bit左にずらして重ねる
    return (r << 16 | g << 8 | b);
}

int parse_config_line(char *line, t_data *data)
{
    // 先頭の空白を飛ばしたポインタを作るが、元の line は変えない
    char *trimmed = line;
    while (*trimmed && (*trimmed == ' ' || (*trimmed >= 9 && *trimmed <= 13)))
        trimmed++;

    // 空行は無視（戻り値 1）
    if (*trimmed == '\0' || *trimmed == '\n')
        return (1);

    // テクスチャ・色の解析
    if (ft_strncmp(trimmed, "NO ", 3) == 0)
        data->texture.no_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
    else if (ft_strncmp(trimmed, "SO ", 3) == 0)
        data->texture.so_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
    else if (ft_strncmp(trimmed, "WE ", 3) == 0)
        data->texture.we_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
    else if (ft_strncmp(trimmed, "EA ", 3) == 0)
        data->texture.ea_path = ft_strtrim(trimmed + 3, " \f\n\r\t\v");
    else if (ft_strncmp(trimmed, "F ", 2) == 0)
        data->texture.floor_color = parse_rgb(trimmed + 2);
    else if (ft_strncmp(trimmed, "C ", 2) == 0)
        data->texture.ceiling_color = parse_rgb(trimmed + 2);
    else
    {
        // ここがポイント！ '1' や '0' が見つかったら「ここはマップだ」と判定
        if (*trimmed == '1' || *trimmed == '0')
            return (0);
        return (1); // それ以外の不明なゴミ行は無視
    }
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

int	move(t_data *data)
{
	double speed = 0.05;
    double dx = 0;
    double dy = 0;

    // --- 1. まず「どの方向にどれだけ動きたいか」だけを計算する ---
    if (data->flg.key_w) {
        dx += data->dirX * speed;
        dy += data->dirY * speed;
    }
    if (data->flg.key_s) {
        dx -= data->dirX * speed;
        dy -= data->dirY * speed;
    }
    if (data->flg.key_a) {
        // ★ここを入れ替えて左右を調整する
        dx += data->dirY * speed;
        dy -= data->dirX * speed;
    }
    if (data->flg.key_d) {
        // ★ここを入れ替えて左右を調整する
        dx -= data->dirY * speed;
        dy += data->dirX * speed;
    }

    // --- 2. 壁判定用のマージン（厚み）を作る ---
    // 動く方向 (dx, dy) がプラスならプラス方向に、マイナスならマイナス方向に壁を見る
    double marginX = (dx >= 0) ? 0.2 : -0.2;
    double marginY = (dy >= 0) ? 0.2 : -0.2;

    // --- 3. 実際に壁がないかチェックして動かす ---
    // X方向のチェック：今いる y の位置で、x方向に (移動量 + 厚み) 先に壁がないか
    if (data->map[(int)data->posY][(int)(data->posX + dx + marginX)] != '1'
		&& data->map[(int)data->posY][(int)(data->posX + dx + marginX)] != 'D')
        data->posX += dx;

    // Y方向のチェック：今いる x の位置で、y方向に (移動量 + 厚み) 先に壁がないか
    if (data->map[(int)(data->posY + dy + marginY)][(int)data->posX] != '1'
		&& data->map[(int)(data->posY + dy + marginY)][(int)data->posX] != 'D')
	{
        data->posY += dy;
	}
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
	if (data->flg.key_esc)
		exit(0);
	return (0);
}
