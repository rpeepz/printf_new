#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_flags
{
	size_t		total;
	size_t		buf_len;
	int			width;
	int			precision;
	int			length;
	int			fd;
	int			space:1;
	int			plus:1;
	int			zero:1;
	int			minus:1;
	int			pound:1;
	int			specifier:8;
	char		buffer[916];
	char		*s_buffer;
}				t_flags;

int				ft_printf(const char *fmt, ...);
void			convert(const char **fmt, t_flags *flags, va_list ap);

void			single_letter(t_flags *flags, va_list ap);
void			string(t_flags *flags, va_list ap);
void			number(t_flags *flags, va_list ap);
void			u_number(t_flags *flags, va_list ap);

void			flush_buffer(t_flags *flags);
void			add_buffer(const char *buf, t_flags *flags);

char			*utoa_base(unsigned long n, int base);
char			*itoa_base(long n, int base);
void			str_to_upper(char *s);
long			convert_length(int length, va_list ap, int type);
char			*number_precision(char *buffer, int precision);

int				get_value(const char **fmt, va_list ap, int init_value);
void			get_flags(const char **fmt, t_flags *flags);
void			get_length(const char **fmt, t_flags *flags);
void			get_specifier(const char **fmt, t_flags *flags);

int				ft_atoi(const char *str);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memset(void *b, int c, size_t len);
char			*ft_strndup(const char *s1, size_t n);
char			*ft_strdup(const char *s1);
char			*ft_strcat(char *s1, const char *s2);
size_t			ft_strlen(const char *str);

#endif