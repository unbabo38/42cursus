# --- 変数定義（レシピの材料） ---
NAME        = zombieHorde
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
RM          = rm -rf

# --- ファイル構成（使う食材） ---
SRCS        = main.cpp Zombie.cpp zombieHorde.cpp
OBJS        = $(SRCS:.cpp=.o)

# --- ルール（調理手順） ---

# 最終目標：実行ファイルを作る
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# 各 .cpp ファイルを .o（オブジェクトファイル）に変換する
# % はワイルドカード。「すべての .cpp に対して同じことをしろ」という命令
%.o: %.cpp Zombie.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ゴミ掃除（ビルド中に出た中間ファイルを消す）
clean:
	$(RM) $(OBJS)

# 完全掃除（実行ファイルまで全部消す）
fclean: clean
	$(RM) $(NAME)

# 作り直し（掃除してから最初からビルド）
re: fclean all

# ファイル名とコマンド名が被っても正しく動くようにするおまじない
.PHONY: all clean fclean re
