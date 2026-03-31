# --- プログラム名 ---
NAME        = unnecessary_violence

# --- コンパイラとフラグ ---
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
RM          = rm -rf

# --- ソースファイル ---
# すべての .cpp ファイルをここに列挙します
SRCS        = main.cpp \
              Weapon.cpp \
              HumanA.cpp \
              HumanB.cpp

# --- オブジェクトファイル (.cpp を .o に変換) ---
OBJS        = $(SRCS:.cpp=.o)

# --- ルール定義 ---

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# 各 .o ファイルを作る際、ヘッダーの変更も検知するように依存関係を設定
%.o: %.cpp Weapon.hpp HumanA.hpp HumanB.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
