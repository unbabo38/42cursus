#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

int main(void) {
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        // 子：読み手を完全に閉じる
        close(fd[0]);
        close(fd[1]);
        _exit(0);
    } else {
        // 親：読み取り口も閉じる
        close(fd[0]);
        // SIGPIPE を無視しない（デフォルト動作）
        // ここで write() → SIGPIPE → プロセス終了
		        sleep(1); // 子が exit して fd[0] を閉じ終わるのを待つ

        write(fd[1], "hello", 5);
        perror("write");  // ここには到達しない
    }
    return 0;
}
