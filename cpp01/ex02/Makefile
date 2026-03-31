CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

# 生成する実行ファイル名
TARGET = brain_study

# ソースファイル
SRCS = main.cpp

# 実行ファイルの生成ルール
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# クリーンアップ（生成されたファイルを削除）
clean:
	rm -f $(TARGET)
