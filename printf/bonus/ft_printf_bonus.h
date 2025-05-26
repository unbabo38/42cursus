#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define FLG_LEFT (1 << 1)
#define FLG_ZERO (1 << 2)
#define FLG_PLUS (1 << 3)
#define FLG_SPACE (1 << 4)
#define FLG_HASH (1 << 5)
#define FLAG_DOT (1 << 6)
typedef struct s_format {
    int flg;
    int width;
    int precision;
	int has_precision;
    char specifier;
} t_format;


// ===== メイン関数 =====
int ft_printf(const char *s, ...);

// ===== フォーマット解析 =====
void scan_line(const char *line, int *i, va_list *ap, t_format *fmt);
void parse_flags(const char *line, int *i, t_format *fmt);
void parse_width(const char *line, int *i, va_list *ap, t_format *fmt);
void parse_precision(const char *line, int *i, va_list *ap, t_format *fmt);
void parse_specifier(const char *line, int *i, t_format *fmt);

// ===== 各フォーマット出力 =====
int print_arg(t_format *fmt, va_list *ap);
int print_char(t_format *fmt, const char c);
int print_string(t_format *fmt, const char *s);
int print_decimal(t_format *fmt, int n);
int print_unsigned_decimal(t_format *fmt, unsigned int n);
int print_hexadecimal_low(t_format *fmt, unsigned int n);
int print_hexadecimal_up(t_format *fmt, unsigned int n);

int print_pointer(t_format *fmt, void *ptr);
int print_percent(t_format *fmt);

// ===== ユーティリティ =====
int     ft_strlen(const char *s);
int     ft_isdigit(int c);
int     ft_atoi(const char *s);
char    *ft_itoa(int n);
char *itoa_base(unsigned int n, const char *base);
// ===== 補助的な関数（必要なら） =====
int     max(int a, int b);

#endif
