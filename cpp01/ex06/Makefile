# --- プログラム名 (課題指定の名前) ---
NAME        = harlFilter

# --- コンパイラとフラグ ---
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
RM          = rm -rf

# --- ファイル構成 ---
SRCS        = main.cpp Harl.cpp
OBJS        = $(SRCS:.cpp=.o)

# --- ルール定義 ---

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp Harl.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
