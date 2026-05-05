#include "../include/cub3d.h"

void check_death(t_data *data)
{
    // プレイヤーとスプライトの距離（三平方の定理）
    double dist = sqrt(pow(data->posX - data->sprite.x, 2) +
                       pow(data->posY - data->sprite.y, 2));

    if (dist < 0.4) // 接触範囲
    {
        printf("\a"); // ビープ音（Linux/Mac環境による）
        printf("KILLED BY BARREL!\n");

        // とりあえず初期位置に戻してゲーム継続させる
        data->posX = 2.0;
        data->posY = 2.0;
		exit(0);
        // あるいは exit(0); で終了
    }
}

void move_sprite(t_data *data)
{
    double enemySpeed = 0.0035; // 速すぎると即死するのでこのくらいから
    double buffer = 0.2;       // 壁やドアとの距離を保つバッファ

    // --- X軸の移動 ---
    double nextX = data->sprite.x;
    if (data->sprite.x < data->posX)
        nextX += enemySpeed;
    else
        nextX -= enemySpeed;

    // 移動先が壁('1')でも閉じたドア('D')でもなければ移動
    if (data->map[(int)data->sprite.y][(int)(nextX + (nextX > data->sprite.x ? buffer : -buffer))] != '1' &&
        data->map[(int)data->sprite.y][(int)(nextX + (nextX > data->sprite.x ? buffer : -buffer))] != 'D')
    {
        data->sprite.x = nextX;
    }

    // --- Y軸の移動 ---
    double nextY = data->sprite.y;
    if (data->sprite.y < data->posY)
        nextY += enemySpeed;
    else
        nextY -= enemySpeed;

    if (data->map[(int)(nextY + (nextY > data->sprite.y ? buffer : -buffer))][(int)data->sprite.x] != '1' &&
        data->map[(int)(nextY + (nextY > data->sprite.y ? buffer : -buffer))][(int)data->sprite.x] != 'D')
    {
        data->sprite.y = nextY;
    }
}

void render_sprites(t_data *data)
{
    // 1. プレイヤーとスプライトの相対距離を計算
    double spr_x = data->sprite.x - data->posX;
    double spr_y = data->sprite.y - data->posY;

    // 2. カメラ行列の逆行列を使用して、プレイヤー基準の座標に変換
    // transformYが「プレイヤーからの前方距離」になります
    double invDet = 1.0 / (data->planeX * data->dirY - data->dirX * data->planeY);
    double transformX = invDet * (data->dirY * spr_x - data->dirX * spr_y);
    double transformY = invDet * (-data->planeY * spr_x + data->planeX * spr_y);

    // 3. 画面上のX座標、高さ、幅を計算
    int spr_screen_x = (int)((SCREEN_WIDTH / 2) * (1 + transformX / transformY));

    // スプライトの大きさを計算（距離transformYに反比例させる）
    int spr_height = abs((int)(SCREEN_HEIGHT / transformY));
    int spr_width = abs((int)(SCREEN_HEIGHT / transformY));

    // 4. 描画範囲の計算（画面外にはみ出さないようにガード）
    int draw_start_y = -spr_height / 2 + SCREEN_HEIGHT / 2;
    if (draw_start_y < 0) draw_start_y = 0;
    int draw_end_y = spr_height / 2 + SCREEN_HEIGHT / 2;
    if (draw_end_y >= SCREEN_HEIGHT) draw_end_y = SCREEN_HEIGHT - 1;

    int draw_start_x = -spr_width / 2 + spr_screen_x;
    if (draw_start_x < 0) draw_start_x = 0;
    int draw_end_x = spr_width / 2 + spr_screen_x;
    if (draw_end_x >= SCREEN_WIDTH) draw_end_x = SCREEN_WIDTH - 1;

    // 5. 実際の描画ループ
    for (int x = draw_start_x; x < draw_end_x; x++)
    {
        int texX = (int)(256 * (x - (-spr_width / 2 + spr_screen_x)) * 64 / spr_width) / 256;

        // 条件1: プレイヤーの目の前にあるか (transformY > 0)
        // 条件2: Zバッファより手前にあるか (transformY < z_buffer[x])
        if (transformY > 0 && transformY < data->z_buffer[x])
        {
            for (int y = draw_start_y; y < draw_end_y; y++)
            {
                int d = y * 256 - SCREEN_HEIGHT * 128 + spr_height * 128;
                int texY = ((d * 64) / spr_height) / 256;

                int color = get_pixel_color(&data->tex[5], texX, texY);
                // 透過色（黒など）を飛ばして描画
                if ((color & 0x00FFFFFF) != 0)
                    my_mlx_pixel_put(data, x, y, color);
            }
        }
    }
}

int mouse_move(int x, int y, t_data *data)
{
	(void)y;
    // static変数で前回のマウス位置を記憶しておく
    static int last_x = -1;
    double rotSpeed;

    // 初回呼び出し時は、現在の位置を保存して終了（回転させない）
    if (last_x == -1)
    {
        last_x = x;
        return (0);
    }

    // 前回の位置との差分を計算
    // x - last_x が正なら右移動、負なら左移動
    rotSpeed = (x - last_x) * 0.01;

    // 回転処理
    double oldDirX = data->dirX;
    data->dirX = data->dirX * cos(rotSpeed) - data->dirY * sin(rotSpeed);
    data->dirY = oldDirX * sin(rotSpeed) + data->dirY * cos(rotSpeed);

    double oldPlaneX = data->planeX;
    data->planeX = data->planeX * cos(rotSpeed) - data->planeY * sin(rotSpeed);
    data->planeY = oldPlaneX * sin(rotSpeed) + data->planeY * cos(rotSpeed);

    // 今回のxを保存
    last_x = x;

    return (0);
}

void    draw_square(t_data *data, int x, int y, int size, int color)
{
    int i;
    int j;

    i = 0;
    while (i < size)
    {
        j = 0;
        while (j < size)
        {
            // 画面外への書き込みを防止
            if (x + i >= 0 && x + i < SCREEN_WIDTH && y + j >= 0 && y + j < SCREEN_HEIGHT)
            {
                my_mlx_pixel_put(data, x + i, y + j, color);
            }
            j++;
        }
        i++;
    }
}

void draw_minimap(t_data *data)
{
    int x, y;
    int size = 5; // 1マスの大きさ（ピクセル）

    y = 0;
    while (y < data->map_height)
    {
        x = 0;
        while (x < data->map_width)
        {
            int color = (data->map[y][x] == '1') ? 0xFFFFFF : 0x000000;
            // 指定した範囲（例：(x*size, y*size)）を塗りつぶす
            draw_square(data, x * size + 20, y * size + 20, size, color);
            x++;
        }
        y++;
    }
    // プレイヤーの現在地を点として描く
    draw_square(data, data->posX * size + 20, data->posY * size + 20, 3, 0xFF0000);
}

unsigned int get_pixel_color(t_img *img, int x, int y)
{
    char    *dst;

    // 画像のサイズ（通常64）を超えないようにガード
    // もし img 構造体に width/height がなければ 64 などの固定値
    if (x < 0 || x >= 64 || y < 0 || y >= 64)
        return (0);
    dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    return (*(unsigned int*)dst);
}

void load_textures(t_data *data)
{
    int w, h;
    char *paths[6];

    // パスを配列にまとめてループで回すとスッキリします
    paths[0] = data->texture.no_path;
    paths[1] = data->texture.so_path;
    paths[2] = data->texture.we_path;
    paths[3] = data->texture.ea_path;
	paths[4] = "textures/door.xpm";
	paths[5] = "textures/barrel.xpm";
	// for (int i = 0; i < 4; i++) {
	// 	if (paths[i]) {
	// 		//printf("Path[%d]: |%s|\n", i, paths[i]);
	// 		// 1文字ずつ16進数で出して、目に見えない文字がないか確認
	// 		for (int j = 0; paths[i][j]; j++)
	// 			//printf("%02x ", (unsigned char)paths[i][j]);
	// 		//printf("\n");
	// 	} else {
	// 		//printf("Path[%d] is NULL!\n", i);
	// 	}
	// }
    for (int i = 0; i < 6; i++)
    {
        // 1. 画像ファイルを読み込む

        data->tex[i].img = mlx_xpm_file_to_image(data->mlx, paths[i], &w, &h);
        if (!data->tex[i].img)
        {
            ///printf("Error: Failed to load texture %s\n", paths[i]);
            exit(1); // 本当はもっと綺麗にfreeして終わるべき
        }
        // 2. 画像の生データへのアドレスを取得する
        data->tex[i].addr = mlx_get_data_addr(data->tex[i].img,
                                              &data->tex[i].bits_per_pixel,
                                              &data->tex[i].line_length,
                                              &data->tex[i].endian);
        // 画像のサイズも後で使うので保存しておくと楽です
        // (t_img 構造体に width/height を追加している場合)
    }
}

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
				my_mlx_pixel_put(data, i, j, data->texture.ceiling_color); // 天井（グレー）
			else
				my_mlx_pixel_put(data, i, j, data->texture.floor_color); // 床（明るいグレー）
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
		// 	printf("OUT OF BOUNDS: x:%d, x:%d\n", mapX, mapY);
		// 	return (0);
		// }
		//printf("Raycast Start: pos(%.2f, %.2f)\n", data->posX, data->posY);
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
				if (data->map[mapY][mapX] == '1')
					hit = 1;
				else if (data->map[mapY][mapX] == 'D')
        			hit = 2;
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

		// int drawStart = 100;
		// int drawEnd = 300;
		// for (int x = drawStart; x < drawEnd; x++)
		// 	my_mlx_pixel_put(data, x, x, 0x00FF00);
		int tex_num; // 0:NO, 1:SO, 2:WE, 3:EA

		if (side == 0) // X軸（左右方向）の壁に当たった ＝ 北か南
		{
			if (rayDirX < 0) tex_num = 0; // 北面(NO)
			else tex_num = 1;             // 南面(SO)
		}
		else // Y軸（上下方向）の壁に当たった ＝ 東か西
		{
			if (rayDirY < 0) tex_num = 2; // 西面(WE)
			else tex_num = 3;             // 東面(EA)
		}
		if (hit == 2)
			tex_num = 4;
		int y = drawStart;
		double wallX; // 壁のヒットした場所（0.0 〜 1.0）
		if (side == 0)
			wallX = data->posY + perpWallDist * rayDirY;
		else
			wallX = data->posX + perpWallDist * rayDirX;
		wallX -= floor(wallX); // 小数点以下だけを取り出す（これが壁内の位置になる）

		// テクスチャ上のX座標を計算（テクスチャ幅が64ピクセルの場合）
		int texX = (int)(wallX * (double)64);

		if (side == 0 && rayDirX > 0) texX = 64 - texX - 1;
		if (side == 1 && rayDirY < 0) texX = 64 - texX - 1;
		double step = 1.0 * 64 / lineHeight; // テクスチャの1ピクセルあたりのステップ幅
		double texPos = (drawStart - SCREEN_HEIGHT / 2 + lineHeight / 2) * step;
		while (y < drawEnd)
		{
			// texY の計算（ここも重要！）
			int texY = (int)texPos & (64 - 1); // 64ピクセルの場合
			texPos += step;
			if (texY < 0) texY = 0;

			// ここで修正した関数と配列を使う！
			int color = get_pixel_color(&data->tex[tex_num], texX, texY);

			// 東西南北で少し色を変えると、角がはっきりしてカッコよくなります
			if (side == 1) color = (color >> 1) & 8355711; // 影をつける演出

			my_mlx_pixel_put(data, x, y, color);
			y++;
		}
		data->z_buffer[x] = perpWallDist;
		x++;
	}
	// ("Now at: map[%d][%d] = '%c', maxwidth= %d, maxheight = %d\n",printf
    // 	(int)data->posY, (int)data->posX, data->map[(int)data->posY][(int)data->posX], data->map_width, data->map_height);
	draw_minimap(data);
	render_sprites(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return 0;
}
