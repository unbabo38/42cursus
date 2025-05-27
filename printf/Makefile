NAME = libftprintf.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

# 通常
SRCS = ft_printf.c scan_line.c print.c
OBJS = $(SRCS:.c=.o)

# ボーナス
SRCS_BONUS = $(wildcard bonus/*.c)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

INCLUDES = ft_printf.h

# 通常ビルド
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# ボーナスビルド
bonus: $(OBJS_BONUS)
	ar rcs $(NAME) $(OBJS_BONUS)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
