NAME    = cub3d
LIBFT_DIR = ./include/libft
LIBFT   = $(LIBFT_DIR)/libft.a

CC      = cc
# -I で libft フォルダも指定することで #include "libft.h" が直接書けるようになります
CFLAGS    = -Wall -Wextra -Werror -I ./include -I $(LIBFT_DIR) -I ./include/gnl
MLX     = -L ./mlx -lmlx -lXext -lX11 -lm
SRCS      = src/main.c \
            src/rendering.c \
            src/map.c \
            src/key_press.c \
            ./include/gnl/get_next_line.c \
            ./include/gnl/get_next_line_utils.c
OBJS    = $(SRCS:.c=.o)

all:
	make -C $(LIBFT_DIR)
	make $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(MLX) -o $(NAME)

# .c から .o を作るルールを明示（CFLAGSが適用されるように）
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ... 以下 clean 等は同じ
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
