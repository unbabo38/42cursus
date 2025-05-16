#include "get_next_line.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    const struct {
        const char *filename;
        const char *expected[10];  // 必要に応じて拡張
        int count;
    } tests[] = {
        { "empty.txt",        { NULL },                                  1 },
        { "newline.txt",      { "\n", NULL },                            2 },
        { "one_char.txt",     { "aaa\n", NULL },                           2 },
        { "1char.txt",        { "0", NULL },                             2 }, // ←★追加
        { "exact_buf.txt",    { "AAAAAAAA", NULL },                      2 },
        { "multi_line.txt",   { "Line1\n", "Line2 longer\n", "Line3\n", NULL }, 4 },
    };

    for (int t = 0; t < sizeof(tests)/sizeof(tests[0]); t++) {
        int fd = open(tests[t].filename, O_RDONLY);
        if (fd < 0) {
            perror(tests[t].filename);
            continue;
        }
        for (int i = 0; i < tests[t].count; i++) {
            char *line = get_next_line(fd);
			if (tests[t].expected[i]) {
				printf("Test file: %s, line %d\n", tests[t].filename, i);
				printf("Expected: \"%s\"\n", tests[t].expected[i]);
				printf("Got     : \"%s\"\n", line);
				assert(line && strcmp(line, tests[t].expected[i]) == 0);
				free(line);
			} else {
                assert(line == NULL);
            }
        }
        close(fd);
    }

    // 複数 FD の交互読み
    int fd1 = open("fd1.txt", O_RDONLY);
    int fd2 = open("fd2.txt", O_RDONLY);
    assert(fd1 >= 0 && fd2 >= 0);
    char *l1 = get_next_line(fd1); assert(l1 && strcmp(l1, "X1\n") == 0); free(l1);
    char *l2 = get_next_line(fd2); assert(l2 && strcmp(l2, "Y\n") == 0); free(l2);
    l1 = get_next_line(fd1); assert(l1 && strcmp(l1, "X2\n") == 0); free(l1);
    assert(get_next_line(fd2) == NULL);
    close(fd1);
    close(fd2);

    printf("All tests passed!\n");
    return 0;
}
