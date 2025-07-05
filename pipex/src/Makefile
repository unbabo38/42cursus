# **************************************************************************** #
#                               PIPEX MAKEFILE                                 #
# **************************************************************************** #

NAME_MANDATORY = pipex
NAME_BONUS     = pipex_bonus
CC             = cc
CFLAGS         = -Wall -Wextra -Werror

# ---------------------------------------------------------------------------- #

MANDATORY_SRCS = pipex.c utils.c \
                 ../libft/ft_split.c ../libft/ft_strjoin.c \
                 ../libft/ft_strncmp.c ../libft/ft_strlen.c ../libft/ft_strchr.c

BONUS_SRCS     = pipex_bonus.c utils.c \
                 ../libft/ft_split.c ../libft/ft_strjoin.c \
                 ../libft/ft_strncmp.c ../libft/ft_strlen.c ../libft/ft_strchr.c

MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)
BONUS_OBJS     = $(BONUS_SRCS:.c=.o)

# ---------------------------------------------------------------------------- #

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
