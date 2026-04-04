# --- プログラム名 ---
NAME        = sed_is_for_losers

# --- コンパイラとフラグ ---
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -pedantic
RM          = rm -rf

# --- ファイル構成 ---
SRCS        = main.cpp
OBJS        = $(SRCS:.cpp=.o)

# --- ルール定義 ---

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) test_file test_file.replace

fclean: clean
	$(RM) $(NAME)

re: fclean all

# --- テスト用の便利コマンド ---
test: $(NAME)
	@echo "Creating a test file..."
	@echo "apple orange apple banana apple" > test_file
	@echo "Running: ./$(NAME) test_file apple STRAWBERRY"
	@./$(NAME) test_file apple STRAWBERRY
	@echo "Result in test_file.replace:"
	@cat test_file.replace

.PHONY: all clean fclean re test
