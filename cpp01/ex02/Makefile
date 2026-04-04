NAME        = brain_study
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -pedantic
RM          = rm -f

# ソースファイルとオブジェクトファイルの定義
SRCS        = main.cpp
OBJS        = $(SRCS:.cpp=.o)

# デフォルトターゲット
all: $(NAME)

# 実行ファイルのリンク
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# 各ソースファイルのコンパイル（依存関係にヘッダーがあれば適宜追加してください）
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

# 再ビルド
re: fclean all

# ターゲットの偽称設定（同名のファイルがあっても動作を保証する）
.PHONY: all clean fclean re
