#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int fd[2];
    char buf[100];

    if (pipe(fd) == -1)
        return 1;

    // 書き込み
    write(fd[1], "hello from myself\n", 18);

    // 読み取り
    read(fd[0], buf, sizeof(buf));
    printf("read from pipe: %s", buf);

    return 0;
}
