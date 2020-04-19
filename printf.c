#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void			fill(t_flags *flags, va_list ap);

void			flush_buffer(t_flags *flags)
{
	write(flags->fd, flags->buffer, flags->buf_len);
	flags->total += flags->buf_len;
	flags->buf_len = 0;
	memset(flags->buffer, 0, sizeof(flags->buffer));
}

void			add_buffer(const char *buf, t_flags * flags)
{
	if (strlen(buf) + flags->buf_len > sizeof(flags->buffer) - 1)
		flush_buffer(flags);
	strcat(flags->buffer, buf);
	flags->buf_len += strlen(buf);
}

void			get_flags(const char **fmt, t_flags *flags)
{
	while (*(*fmt))
	{
		if (*(*fmt) == ' ')
			flags->space |= 1;
		else if (*(*fmt) == '+')
			flags->plus |= 1;
		else if (*(*fmt) == '0')
			flags->zero |= 1;
		else if (*(*fmt) == '-')
			flags->minus |= 1;
		else if (*(*fmt) == '#')
			flags->pound |= 1;
		else
			break ;
		++(*fmt);
	}
}

int				get_value(const char **fmt, va_list ap, int init_value)
{
	int		number;

	number = init_value;
	if (*(*fmt) == '*')
	{
		number = va_arg(ap, int);
		++(*fmt);
	}
	else if ((init_value ? *(*fmt) >= '0' : *(*fmt) > '0') && *(*fmt) <= '9')
	{
		number = atoi(*fmt);
		while (*(*fmt) && *(*fmt) >= '0' && *(*fmt) <= '9')
			++(*fmt);
	}
	return (number);
}

void			get_length(const char **fmt, t_flags *flags)
{
	flags->length = 0;
	if (*(*fmt) == 'L')
		flags->length = (int)'L';
	else if (*(*fmt) == 'h' && *(*fmt + 1) != 'h')
		flags->length = (int)'h';
	else if (*(*fmt) == 'h' && *(*fmt + 1) == 'h')
		flags->length = (int)('h' + 'h');
	else if (*(*fmt) == 'l' && *(*fmt + 1) != 'l')
		flags->length = (int)'l';
	else if (*(*fmt) == 'l' && *(*fmt + 1) == 'l')
		flags->length = (int)('l' + 'l');
	else if (*(*fmt) == 'z')
		flags->length = (int)'z';
	if (flags->length > 0)
		*fmt += (flags->length >= (int)('h' + 'h')) ? 2 : 1;
}

void			get_specifier(const char **fmt, t_flags *flags)
{
	char	*specs;
	int		i;

	flags->specifier = 0;
	specs = "%dicspoOuUxXfFbB";
	i = -1;
	while (specs[++i])
	{
		if (*(*fmt) == specs[i])
		{
			flags->specifier = specs[i];
			break ;
		}
	}
	++(*fmt);
}

void			convert(const char **fmt, t_flags *flags, va_list ap)
{
	++(*fmt);
	get_flags(fmt, flags);
	flags->width = get_value(fmt, ap, 0);
	if (*(*fmt) == '.')
		++(*fmt);
	flags->precision = get_value(fmt, ap, -1);
	get_length(fmt, flags);
	get_specifier(fmt, flags);
	fill(flags, ap);
}

#include <stdio.h>
int				ft_printf(const char *fmt, ...)
{
	t_flags	flags;
	va_list	ap;

	memset(&flags, 0, sizeof(flags));
	flags.fd = 1;
	va_start(ap, fmt);
	while (*fmt)
	{
		if (flags.buf_len == sizeof(flags.buffer) - 1)
			flush_buffer(&flags);
		if (*fmt == '%')
			convert(&fmt, &flags, ap);
		else
			flags.buffer[flags.buf_len++] = *fmt++;
	}
	flush_buffer(&flags);
	// printf("fd %d\n", flags.fd);
	// printf("length %d\n", flags.length);
	// printf("flag %d\n", flags.space);
	// printf("flag %d\n", flags.plus);
	// printf("flag %d\n", flags.zero);
	// printf("flag %d\n", flags.minus);
	// printf("flag %d\n", flags.pound);
	// printf("prec %d\n", flags.precision);
	// printf("wid %d\n", flags.width);
	// printf("spec %d\n", flags.specifier);
	return flags.total;
}

void		string(t_flags *flags, va_list ap)
{
	int		len;
	char	*tmp;
	char	*v;

	tmp = NULL;
	v = va_arg(ap, char *);
	v = !v ? "(null)" : v;
	v = (flags->precision > -1 && flags->precision < (int)strlen(v)) ? strndup(v, flags->precision) : strdup(v);
	len = strlen(v);
	if (len < flags->width)
	{
		tmp = malloc(flags->width + 1);
		memset(tmp, flags->zero ? '0' : ' ', flags->width);
		tmp[flags->width] = 0;
		memcpy(tmp + (flags->minus ? 0 : (flags->width - len)), v, len);
		add_buffer(tmp, flags);
		free(tmp);
	}
	else
		add_buffer(v, flags);
	free(v);
}

void		single_letter(t_flags *flags, va_list ap)
{
	int		v;
	char	buffer[flags->width ? flags->width + 1 : 2];

	buffer[flags->width ? flags->width : 1] = 0;
	memset(buffer, flags->zero ? '0' : ' ', flags->width);
	v = flags->specifier == '%' ? '%' : va_arg(ap, int);
	if (flags->minus || flags->width == 1)
		buffer[0] = v;
	else
		buffer[flags->width ? flags->width - 1 : 0] = v;
	strcat(flags->buffer, buffer);
	flags->buf_len += strlen(buffer);
}

// "%dicspoOuUxXfFbB"
void		fill(t_flags *flags, va_list ap)
{

	if (flags->specifier == '%' || flags->specifier == 'c')
		single_letter(flags, ap);
	else if (flags->specifier == 's')
		string(flags, ap);
	else if (flags->specifier == 'p')
		;
	else if (flags->specifier == 'd' || flags->specifier == 'i')
		;
	else if (flags->specifier == 'o' || flags->specifier == 'O')
		;
	else if (flags->specifier == 'u' || flags->specifier == 'U')
		;
	else if (flags->specifier == 'x' || flags->specifier == 'X')
		;
	else if (flags->specifier == 'f' || flags->specifier == 'F')
		;
	else if (flags->specifier == 'b' || flags->specifier == 'B')
		;
}

int		main(void)
{
	int num = ft_printf("%-20.40st%c\n", "can this prin", '?');
	printf ("%d\n", num);
	system("leaks printf");
	return 0;
}
