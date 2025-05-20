#!/bin/bash

# 空ファイル
echo -n "" > empty.txt


# 改行のみ
echo "" > only_nl.txt

# 1文字のみ（改行なし）
echo -n "1" > 1char.txt

# 1文字＋改行
echo -n "a\n" | sed 's/\\n/\'$'\n/' > 1char_nl.txt

# 改行なしの短い文字列
echo -n "abc" > no_nl.txt

# 複数行
printf "line1\nline2\nline3\n" > multi.txt

# 改行だけ複数
printf "\n\n\n" > many_nl.txt

# 巨大な1行（改行あり）
yes A | head -n 100000 | tr -d '\n' | sed 's/$/\n/' > giant_line.txt

# 巨大な1行（改行なし）
yes A | head -n 100000 | tr -d '\n' > giant_line_no_nl.txt

echo "✅ テストファイルが生成されました。"
