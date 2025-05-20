#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char	*get_next_line(int fd);  // 先ほどの gnl 実装をリンク

int main(void)
{
	int     fd;
	char    *line;

	// テストファイル作成
	fd = open("1char_test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open for write");
		return (1);
	}
	write(fd, "0", 1);  // 1文字だけ書き込む（改行なし）
	close(fd);

	// 読み込みテスト開始
	fd = open("1char_test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("open for read");
		return (1);
	}
	printf("===== 1char test =====\n");

	line = get_next_line(fd);
	if (line)
	{
		printf("Returned: \"%s\"\n", line);
		free(line);
	}
	else
		printf("Returned NULL (unexpected)\n");

	line = get_next_line(fd);
	if (line)
	{
		printf("Returned: \"%s\" (should be NULL)\n", line);
		free(line);
	}
	else
		printf("Returned NULL as expected\n");

	close(fd);
	return (0);
}
