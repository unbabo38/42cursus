# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/05 21:43:13 by tmura             #+#    #+#              #
#    Updated: 2025/07/06 13:29:32 by tmura            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_MANDATORY = pipex
NAME_BONUS     = pipex_bonus
CC             = cc
CFLAGS         = -Wall -Wextra -Werror


MANDATORY_SRCS = pipex.c utils.c error_massages.c process_exec.c\
                 ../libft/ft_split.c ../libft/ft_strjoin.c \
                 ../libft/ft_strncmp.c ../libft/ft_strlen.c ../libft/ft_strchr.c

BONUS_SRCS     = pipex_bonus.c utils.c error_massages.c here_doc.c process_exec.c\
                 ../libft/ft_split.c ../libft/ft_strjoin.c \
                 ../libft/ft_strncmp.c ../libft/ft_strlen.c ../libft/ft_strchr.c \
				 ../libft/ft_strdup.c ../libft/ft_isalnum.c ../libft/ft_memcpy.c\
				 ../libft/ft_isdigit.c ../libft/ft_isalpha.c

MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)
BONUS_OBJS     = $(BONUS_SRCS:.c=.o)


all: $(NAME_MANDATORY)

bonus: $(NAME_BONUS)

$(NAME_MANDATORY): $(MANDATORY_OBJS)
	$(CC) $(CFLAGS) $(MANDATORY_OBJS) -o $(NAME_MANDATORY)

$(NAME_BONUS): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(NAME_BONUS)

clean:
	rm -f $(MANDATORY_OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME_MANDATORY) $(NAME_BONUS)

re: fclean all

.PHONY: all bonus clean fclean re
