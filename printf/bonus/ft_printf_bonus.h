#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
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

typedef struct s_render_info {
	char	*str;
	int		len;
	int		sign_len;
	char	sign_char;
	int		pad_zero;
	int		pad_space;
	int		prefix_len;
	const char *prefix_str;  // ← これを追加すれば汎用OK
	int		total_len;
} t_render_info;

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
int print_integer(t_format *fmt, int n);
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
char 	*itoa_base(long long n, const char *base);
char 	*itoa_base_unsigned(unsigned long long n, const char *base);

// ===== 補助的な関数（必要なら） =====
int     max(int a, int b);

/*
t_render_info compute_render_info(t_format *fmt, int decimal);
void 	render_integer(t_format *fmt, t_render_info *info);
void 	render_left_aligned_integer(t_render_info *info);
void 	render_right_aligned_integer(t_format *fmt, t_render_info *info);
void 	render_right_zero_padded(t_render_info *info);
void 	render_right_space_padded(t_render_info *info);
*/
t_render_info compute_render_info(t_format *fmt, int decimal);
void render_output(t_format *fmt, t_render_info *info);
void render_left(t_render_info *info);
void render_right_zero(t_render_info *info);
void render_right_space(t_render_info *info);

t_render_info compute_hex_render_info(t_format *fmt, unsigned int value);
t_render_info compute_uphex_render_info(t_format *fmt, unsigned int value);
t_render_info compute_pointer_render_info(t_format *fmt, void *ptr);
t_render_info compute_unsigned_decimal_render_info(t_format *fmt, int decimal);


/*
int print_hexadecimal_low(t_format *fmt, unsigned int hex_decimal_low);
void render_hexadecimal_low(t_format *fmt, t_render_info *info);
void render_right_aligned_hex(t_format *fmt, t_render_info *info);
void render_right_zero_padded_hex(t_render_info *info);
void render_right_space_padded_hex(t_render_info *info);
void render_left_aligned_hex(t_render_info *info);
void			render_hexadecimal_low(t_format *fmt, t_render_info *info);
void			render_left_aligned_hex(t_render_info *info);
void			render_right_aligned_hex(t_format *fmt, t_render_info *info);
void			render_right_zero_padded_hex(t_render_info *info);
void			render_right_space_padded_hex(t_render_info *info);
*/

char sign_check(t_format *fmt, int decimal);
extern const char *low_base;
extern const char *up_base;
extern const char *ten_base;



t_render_info	compute_unsigned_render_info(
	t_format *fmt,
	unsigned long long val,
	const char *base,
	const char *prefix,
	int use_prefix_when_zero
);

t_render_info	compute_signed_render_info(t_format *fmt, int val);


#endif
