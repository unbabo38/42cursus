# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: YOURNAME <your@email.com>                   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/25                                #+#    #+#              #
#    Updated: 2025/05/25                                ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

# ボーナスディレクトリ配下の .c ファイルだけを対象にする
BONUS_DIR = bonus

SRCS = $(BONUS_DIR)/ft_printf_bonus.c \
       $(BONUS_DIR)/scan_line_bonus.c \
       $(BONUS_DIR)/print_bonus.c \
       $(BONUS_DIR)/print_hexadecimal_low_bonus.c \
       $(BONUS_DIR)/print_integer_bonus.c

OBJS = $(SRCS:.c=.o)

INCLUDES = $(BONUS_DIR)/ft_printf_bonus.h

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

bonus: $(BONUS_OBJS)
	ar rcs $(NAME) $(BONUS_OBJS)
# .o ファイルは bonus ディレクトリに依存（.oはカレントに作られる）
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -I $(BONUS_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
