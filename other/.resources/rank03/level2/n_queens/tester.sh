#!/bin/bash
source ../../../main/colors.sh

rendu_dir="../../../../rendu/n_queens"

# -------------------------------------------------------------
# 1. Collect C files
# -------------------------------------------------------------
if [ ! -d "$rendu_dir" ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: Directory $rendu_dir does not exist$(tput sgr 0)"
    exit 1
fi

c_files=$(find "$rendu_dir" -name "*.c" -type f)
if [ -z "$c_files" ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: No .c files found in $rendu_dir$(tput sgr 0)"
    exit 1
fi

# -------------------------------------------------------------
# 2. Compile program BEFORE running tests
# -------------------------------------------------------------
echo "${BLUE}Compiling n_queens program...${RESET}"
gcc -Wall -Werror -Wextra -o n_queens_test $c_files 2>/dev/null
if [ $? -ne 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: Compilation error$(tput sgr 0)"
    exit 1
fi

# -------------------------------------------------------------
# 3. Test: n=4 (sub.txt example)
# -------------------------------------------------------------
echo "${BLUE}Testing n=4 (sub.txt example)...${RESET}"
./n_queens_test 4 > output2.txt 2>/dev/null
lines=$(wc -l < output2.txt)
if [ $lines -ne 2 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=4 should have exactly 2 solutions as specified in sub.txt, got $lines$(tput sgr 0)"
    cat output2.txt
    rm -f n_queens_test output*.txt
    exit 1
fi

expected_solutions=("1 3 0 2" "2 0 3 1")
while IFS= read -r line; do
    found=false
    for expected in "${expected_solutions[@]}"; do
        if [ "$line" = "$expected" ]; then
            found=true
            break
        fi
    done
    if [ "$found" = false ]; then
        echo "$(tput setaf 1)$(tput bold)FAIL: Unexpected solution for n=4: '$line'$(tput sgr 0)"
        echo "Expected: ${expected_solutions[*]}"
        rm -f n_queens_test output*.txt
        exit 1
    fi
done < output2.txt

# -------------------------------------------------------------
# 4. n=2 → 0 solution
# -------------------------------------------------------------
echo "${BLUE}Testing n=2...${RESET}"
./n_queens_test 2 > output2a.txt 2>/dev/null
lines=$(wc -l < output2a.txt)
if [ $lines -ne 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=2 should have 0 solutions, got $lines$(tput sgr 0)"
    cat output2a.txt
    rm -f n_queens_test output*.txt
    exit 1
fi

# -------------------------------------------------------------
# 5. n=3 → 0 solution
# -------------------------------------------------------------
echo "${BLUE}Testing n=3...${RESET}"
./n_queens_test 3 > output3.txt 2>/dev/null
lines=$(wc -l < output3.txt)
if [ $lines -ne 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=3 should have 0 solutions, got $lines$(tput sgr 0)"
    cat output3.txt
    rm -f n_queens_test output*.txt
    exit 1
fi

# -------------------------------------------------------------
# 6. n=4 → 2 solutions (again)
# -------------------------------------------------------------
echo "${BLUE}Testing n=4...${RESET}"
./n_queens_test 4 > output4.txt 2>/dev/null
lines=$(wc -l < output4.txt)
if [ $lines -ne 2 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=4 should have exactly 2 solutions, got $lines$(tput sgr 0)"
    cat output4.txt
    rm -f n_queens_test output*.txt
    exit 1
fi

# -------------------------------------------------------------
# 7. n=8 → 92 solutions
# -------------------------------------------------------------
echo "${BLUE}Testing n=8...${RESET}"
./n_queens_test 8 > output8.txt 2>/dev/null
lines=$(wc -l < output8.txt)
if [ $lines -ne 92 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=8 should have 92 solutions, got $lines$(tput sgr 0)"
    rm -f n_queens_test output*.txt
    exit 1
fi

# -------------------------------------------------------------
# 8. n=0 → 0 solution
# -------------------------------------------------------------
echo "${BLUE}Testing n=0...${RESET}"
./n_queens_test 0 > output0.txt 2>/dev/null
lines=$(wc -l < output0.txt)
if [ $lines -ne 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=0 should have 0 solutions, got $lines$(tput sgr 0)"
    rm -f n_queens_test output*.txt
    exit 1
fi

# -------------------------------------------------------------
# 9. Wrong argument count
# -------------------------------------------------------------
echo "${BLUE}Testing wrong argument count...${RESET}"
./n_queens_test > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: Should fail with no arguments$(tput sgr 0)"
    exit 1
fi

./n_queens_test 1 2 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "$(tput setaf 1)$(tput bold)FAIL: Should fail with too many arguments$(tput sgr 0)"
    exit 1
fi

# -------------------------------------------------------------
# 10. Validate exact n=4 solutions
# -------------------------------------------------------------
echo "${BLUE}Validating n=4 solutions precisely...${RESET}"
sort output4.txt > sorted4.txt
echo -e "1 3 0 2\n2 0 3 1" | sort > expected4.txt

if ! diff -q sorted4.txt expected4.txt >/dev/null; then
    echo "$(tput setaf 1)$(tput bold)FAIL: n=4 solutions mismatch$(tput sgr 0)"
    echo "Expected:"; cat expected4.txt
    echo "Got:"; cat sorted4.txt
    rm -f n_queens_test output*.txt sorted4.txt expected4.txt
    exit 1
fi

# -------------------------------------------------------------
# Cleanup
# -------------------------------------------------------------
rm -f n_queens_test output*.txt sorted4.txt expected4.txt

echo "$(tput setaf 2)$(tput bold)PASSED 🎉$(tput sgr 0)"
exit 0
