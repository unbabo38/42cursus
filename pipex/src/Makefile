# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 21:43:13 by tmura             #+#    #+#              #
#    Updated: 2025/07/08 15:11:52 by tmura            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME       := pipex
CC         := cc
CFLAGS     := -Wall -Wextra -Werror

# ソースファイル定義
MANDATORY_SRCS := \
	pipex.c \
	utils.c \
	error_massages.c \
	process_exec.c \
	../libft/ft_split.c \
	../libft/ft_strjoin.c \
	../libft/ft_strncmp.c \
	../libft/ft_strlen.c \
	../libft/ft_strchr.c

BONUS_SRCS := \
	pipex_bonus.c \
	utils.c \
	error_massages.c \
	here_doc.c \
	here_doc_expand.c \
	process_exec.c \
	../libft/ft_split.c \
	../libft/ft_strjoin.c \
	../libft/ft_strncmp.c \
	../libft/ft_strlen.c \
	../libft/ft_strchr.c \
	../libft/ft_strdup.c \
	../libft/ft_isalnum.c \
	../libft/ft_memcpy.c \
	../libft/ft_isdigit.c \
	../libft/ft_isalpha.c

OBJS_MAND := $(MANDATORY_SRCS:.c=.o)
OBJS_BONUS := $(BONUS_SRCS:.c=.o)

.PHONY: all mandatory bonus clean fclean re

all: mandatory

# mandatory ビルド
mandatory: $(OBJS_MAND)
	$(CC) $(CFLAGS) $^ -o $(NAME)

# bonus ビルド（同じ pipex バイナリを上書き）
bonus: $(OBJS_BONUS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

clean:
	rm -f $(OBJS_MAND) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all
