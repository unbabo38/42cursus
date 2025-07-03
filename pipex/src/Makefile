# **************************************************************************** #
#                               PIPEX BONUS                                    #
# **************************************************************************** #

NAME        = pipex_bonus
CC          = cc
CFLAGS      = -Wall -Wextra -Werror

# ---------------------------------------------------------------------------- #

SRCS        = pipex_bonus.c utils.c \
              ../libft/ft_split.c ../libft/ft_strjoin.c \
              ../libft/ft_strncmp.c ../libft/ft_strlen.c ../libft/ft_strchr.c

OBJS        = $(SRCS:.c=.o)

# ---------------------------------------------------------------------------- #

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
