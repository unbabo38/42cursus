#!/usr/bin/env bash
set -eu

# 実行バイナリと出力ファイル
PIPEX=./pipex
OUT=out.txt
LEAK_OPTS="--leak-check=full --show-leak-kinds=all --track-fds=yes"

########################################
# here-doc 用テスト関数            #
########################################
test_leak_hd() {
  local limiter=$1; shift
  local cmd=("$@")
  echo
  echo "==== HD TEST: limiter='${limiter}' cmds='${cmd[*]}' ===="
  valgrind $LEAK_OPTS \
    $PIPEX here_doc "$limiter" "${cmd[@]}" "$OUT" <<EOF
foo
bar
${limiter}
EOF
}

# ── here-doc テストケース ──
test_leak_hd "END" cat "wc -l"
test_leak_hd "END" cat "wc -l" <<EOF
END
EOF
test_leak_hd "X" cat "wc -l" <<EOF

X
EOF
test_leak_hd "STOP" cat "wc -l" <<EOF
$(for i in $(seq 1 10000); do echo "Line $i"; done)
STOP
EOF
test_leak_hd "NOEXP" bash -c 'echo $HOME; echo $PATH' "wc -l"

########################################
# multipipe 用テスト関数            #
########################################
test_leak_mp() {
  local infile=$1; shift
  local cmds=("$@")
  echo
  echo "==== MP TEST: infile='${infile}' cmds='${cmds[*]}' ===="
  valgrind $LEAK_OPTS \
    $PIPEX "$infile" "${cmds[@]}" "$OUT"
}

# multipipe 3 コマンド
# cat infile -> grep foo -> wc -l
echo -e "foo\nbar\nfoo" > infile1.txt
test_leak_mp infile1.txt cat "grep foo" "wc -l"

# multipipe 4 コマンド
# seq 1-100 -> grep [2-4] -> sort -r -> head -n 2
echo -e $(seq -s '\n' 1 100) > infile2.txt
test_leak_mp infile2.txt cat "grep '[2-4]'" "sort -r" "head -n 2"

# multipipe 5 コマンド
# cat infile -> cut -d' ' -f2 -> awk -> uniq -> wc -c
echo -e "A a\nB b\nA a" > infile3.txt
test_leak_mp infile3.txt cat "cut -d' ' -f2" "awk '{print \$1}'" "uniq" "wc -c"

# 標準入力利用ケース
echo -e "alpha\nbeta" > infile4.txt
test_leak_mp - cat "grep a" "wc -l"

# 大量ケース: 1000行 -> 3コマンド
echo -e "$(for i in $(seq 1 1000); do echo "Item $i"; done)" > infile5.txt
test_leak_mp infile5.txt cat "grep Item" "wc -l"
