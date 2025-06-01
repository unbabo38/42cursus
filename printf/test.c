#include <stdio.h>
#include <limits.h>
#include "./include/ft_printf.h"
int main(){

	  //ft_printf(NULL);
	  //ft_printf("%s","あいうえお");
	  //ft_printf("%d",ft_printf(NULL));
 	  //printf("%d", printf("%%%%%", "ok"));
	//int ret = ft_printf("%.c",'t');
	//printf(" => %d\n", ret);
	//ret = printf("org: |%k|", 42); // undefined behavior
	//printf(" => %d\n", ret);
	  //printf("%d",ft_printf("%bc%k%bes"));
	  //printf("\n");
	  //printf("%d",printf("%bc%k%bes"));
	  //printf("%d\n",ft_printf("%cc"));
	  //printf("%d",printf("%cc"));


	  //printf("%d", printf(NULL));
	  //printf("%d", ft_printf("%s","あいうえお"));
	  //printf("%d",ft_printf(NULL));
	  //ft_printf("%s\n%s\n",__INT_MAX__,"ssii");
	  //printf("%s\n%s\n",__INT_MAX__,"ssii");

	int ret;

    // 1. フォーマット文字列が NULL → UB
    //ret = printf(NULL);
    //printf("ret: %d\n", ret);

    // 2. %s に NULL → UB（多くの実装でセグフォ）
    //char *null_str = NULL;
    // ret = printf("%s\n", null_str);
    // printf("ret: %d\n", ret);

    // 3. %d に char* を渡す → UB
    // ret = printf("%d\n", "123");
    // printf("ret: %d\n", ret);

    // 4. 引数不足（%d があるのに引数なし） → UB
    // ret = printf("%d\n");
    // printf("ret: %d\n", ret);

    // 5. % だけで終わってる → UB
    // ret = printf("%10");
    // printf("ret: %d\n", ret);

    // 6. 幅指定が異常に大きい → UBまたはheapエラー
    // ret = printf("%999999999d\n", 42);
    // printf("ret: %d\n", ret);

    // 7. % が奇数個 → UB（出力されずに戻る or crash）
    // ret = printf("%%%\n");
    // printf("ret: %d\n", ret);

    // 8. %*d に非常に大きい幅 → UB
    // ret = printf("%*d\n", 1000000000, 42);
    // printf("ret: %d\n", ret);

    // 9. %c に無効な値（非ASCIIなど） → UBの可能性
    //ret = printf("%c\n", 9999999);
    //printf("ret: %d\n", ret);

    // 10. long long を int にキャストして %d で出力 → 情報落ち + UBの可能性
    //long long big = 1234567890123456789LL;
    //ret = ft_printf("%d\n", (int)big);
    //printf("ret: %d\n", ret);

	//int ret;
/*
	// 基本文字列
	ret = ft_printf("ft : |abcdef|");
	ft_printf(" => %d\n", ret);
	ret = printf("org: |abcdef|");
	printf(" => %d\n", ret);

	ret = ft_printf("ft : ||");
	ft_printf(" => %d\n", ret);
	ret = printf("org: ||");
	printf(" => %d\n", ret);

	// 文字
	ret = ft_printf("ft : |abc%cdef|", '0');
	ft_printf(" => %d\n", ret);
	ret = printf("org: |abc%cdef|", '0');
	printf(" => %d\n", ret);

	ret = ft_printf("ft : |abc%5cdef|", 'X');
	ft_printf(" => %d\n", ret);
	ret = printf("org: |abc%5cdef|", 'X');
	printf(" => %d\n", ret);

	ret = ft_printf("ft : |abc%-5cdef|", 'X');
	ft_printf(" => %d\n", ret);
	ret = printf("org: |abc%-5cdef|", 'X');
	printf(" => %d\n", ret);

	ret = ft_printf("ft : |abc%0+ #-10.5cdef|", 'X');
	ft_printf(" => %d\n", ret);
	ret = printf("org: |abc%0+ #-10.5cdef|", 'X');
	printf(" => %d\n", ret);

	// 数値（int）
	ret = ft_printf("ft : abc%ddef\n", 10);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%ddef\n", 10);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%4ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%4ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%-4ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%-4ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%04ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%04ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%10.5ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%10.5ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%0-10.5ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%0-10.5ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%0-+10.5ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%0-+10.5ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%.0ddef\n", 12);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%.0ddef\n", 12);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%.0ddef\n", 0);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%.0ddef\n", 0);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%+15.5ddef\n", INT_MIN);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%+15.5ddef\n", INT_MIN);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%-010ddef\n", 100);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%-010ddef\n", 100);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc% ddef\n", -100);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc% ddef\n", -100);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc% ddef\n", 100);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc% ddef\n", 100);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%+ ddef\n", 100);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%+ ddef\n", 100);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : +%5d+\n", 200);
	ft_printf(" => %d\n", ret);
	ret = printf("org: +%5d+\n", 200);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : +%05d+\n", 200);
	ft_printf(" => %d\n", ret);
	ret = printf("org: +%05d+\n", 200);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : +%-05d+\n", 200);
	ft_printf(" => %d\n", ret);
	ret = printf("org: +%-05d+\n", 200);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : +%010.6d+\n", 200);
	ft_printf(" => %d\n", ret);
	ret = printf("org: +%010.6d+\n", 200);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : +%0.6d+\n", 200);
	ft_printf(" => %d\n", ret);
	ret = printf("org: +%0.6d+\n", 200);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : +%-010.6d+\n", 200);
	ft_printf(" => %d\n", ret);
	ret = printf("org: +%-010.6d+\n", 200);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc% ddef\n", 10);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc% ddef\n", 10);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc% ddef\n", -10);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc% ddef\n", -10);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc% +ddef\n", 10);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc% +ddef\n", 10);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : %.d\n", 5);
	ft_printf(" => %d\n", ret);
	ret = printf("org: %.d\n", 5);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : %.d\n", 0);
	ft_printf(" => %d\n", ret);
	ret = printf("org: %.d\n", 0);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%010ddef\n", -10);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%010ddef\n", -10);
	printf(" => %d\n", ret);

	ret = ft_printf("ft : abc%0-10ddef\n", -10);
	ft_printf(" => %d\n", ret);
	ret = printf("org: abc%0-10ddef\n", -10);
	printf(" => %d\n", ret);
*/

	// 未対応のフォーマット指定子（%k など）
	//ret = ft_printf("ft : |%kfaoewijfaioew|\n", 42);
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%kfaoewijfaioew|\n", 42); // undefined behavior
	//printf(" => %d\n", ret);
//
	//// NULL ポインタを %s に渡す
	//char *null_str = NULL;
	//ret = ft_printf("ft : |%s|\n", null_str);
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%s|\n", null_str); // usually prints (null)
	//printf(" => %d\n", ret);
//
	//// NULL ポインタを %p に渡す
	//void *null_ptr = NULL;
	//ret = ft_printf("ft : |%p|\n", null_ptr);
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%p|\n", null_ptr);
	//printf(" => %d\n", ret);
//
	//// 不正なフォーマット文字列：フォーマットが途中で切れる
	//ret = ft_printf("ft : |abc %");
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |abc %");
	//printf(" => %d\n", ret);
//
	//// フラグだけで変換指定子が無いケース
	//ret = ft_printf("ft : |%-+ #0|\n");
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%-+ #0|\n");
	//printf(" => %d\n", ret);
//
	//// INT_MIN に対する %d
	//ret = ft_printf("ft : |%d|\n", INT_MIN);
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%d|\n", INT_MIN);
	//printf(" => %d\n", ret);
//
	//// INT_MAX に対する %d
	//ret = ft_printf("ft : |%d|\n", INT_MAX);
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%d|\n", INT_MAX);
	//printf(" => %d\n", ret);
//
	//// 0幅・0精度で NULL を出力（%s）
	//ret = ft_printf("ft : |%0.0s|\n", NULL);
	//ft_printf(" => %d\n", ret);
	//ret = printf("org: |%0.0s|\n", NULL);
	//printf(" => %d\n", ret);

	// 長すぎる（無限長）フォーマット文字列（理論テスト）
	// char very_long_fmt[10000];
	// memset(very_long_fmt, 'x', 9999);
	// very_long_fmt[9999] = '\0';
	// ret = ft_printf("ft : |%s|\n", very_long_fmt);
	// ft_printf(" => %d\n", ret);
	// ret = printf("org: |%s|\n", very_long_fmt);
	// printf(" => %d\n", ret);

	return 0;


}
