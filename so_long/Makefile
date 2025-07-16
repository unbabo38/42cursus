# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/12 23:39:52 by tmura             #+#    #+#              #
#    Updated: 2025/07/14 17:18:39 by tmura            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME       := so_long
BONUSNAME  := so_long_bonus

CC         := cc
CFLAGS     := -Wall -Wextra -Werror

LIBFTDIR   := libft
LIBFT      := $(LIBFTDIR)/libft.a
MLXDIR     := minilibx-linux
MLX        := $(MLXDIR)/libmlx.a

INCLUDES   := -Iinclude -I$(LIBFTDIR)/include -I$(MLXDIR)
MLXFLAGS   := -L$(MLXDIR) -lmlx -lXext -lX11 -lm
TMPDIR     := ar_tmp

MANDATORY_SRCS := \
	src/mandatory/main.c \
	src/mandatory/check_input.c \
	src/mandatory/check_wall.c \
	src/mandatory/check_elements.c \
	src/mandatory/free.c \
	src/mandatory/errors.c \
	src/mandatory/utility.c \
	src/mandatory/mlx.c \
	src/mandatory/load_images.c \
	src/mandatory/draw.c \
	src/mandatory/move.c \
	src/mandatory/move_enemy.c \
	src/mandatory/is_valid_map.c \
	src/mandatory/update_frame.c \
	gnl/get_next_line.c \
	gnl/get_next_line_utils.c


MANDATORY_OBJS := $(MANDATORY_SRCS:.c=.o)
BONUS_OBJS     := $(BONUS_SRCS:.c=.o)

.PHONY: all bonus clean fclean re

# ---------- Mandatory ----------

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(MANDATORY_OBJS)
	@mkdir -p $(TMPDIR)
	@cp $(LIBFT) $(TMPDIR)
	@cd $(TMPDIR) && ar x ../$(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(MANDATORY_OBJS) $(TMPDIR)/*.o -o $(NAME) $(MLXFLAGS)
	@rm -rf $(TMPDIR)

# ---------- Bonus ----------

bonus: $(BONUSNAME)

$(BONUSNAME): $(LIBFT) $(MLX) $(BONUS_OBJS)
	@mkdir -p $(TMPDIR)
	@cp $(LIBFT) $(TMPDIR)
	@cd $(TMPDIR) && ar x ../$(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(BONUS_OBJS) $(TMPDIR)/*.o -o $(BONUSNAME) $(MLXFLAGS)
	@rm -rf $(TMPDIR)

# ---------- Dependencies ----------

$(LIBFT):
	@$(MAKE) -s -C $(LIBFTDIR)

$(MLX):
	@$(MAKE) -s -C $(MLXDIR)

# ---------- Compilation Rule ----------

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ---------- Clean ----------

clean:
	@$(MAKE) -C $(LIBFTDIR) clean
	@$(MAKE) -C $(MLXDIR) clean
	@rm -f $(MANDATORY_OBJS) $(BONUS_OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFTDIR) fclean
	@rm -f $(NAME) $(BONUSNAME)

re: fclean all
