#include "ft_printf.h"

int		ft_printf(const char *fmt, ...)
{
	t_flags	flags;
	va_list	ap;

	ft_memset(&flags, 0, sizeof(flags));
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
	return flags.total;
}

void	convert(const char **fmt, t_flags *flags, va_list ap)
{
	++(*fmt);
	get_flags(fmt, flags);
	flags->width = get_value(fmt, ap, 0);
	if (*(*fmt) == '.')
		++(*fmt);
	flags->precision = get_value(fmt, ap, -1);
	get_length(fmt, flags);
	get_specifier(fmt, flags);
	if (flags->specifier == '%' || flags->specifier == 'c')
		single_letter(flags, ap);
	else if (flags->specifier == 's')
		string(flags, ap);
	else if (flags->specifier == 'd' || flags->specifier == 'i')
		number(flags, ap);
	else if (flags->specifier == 'u' || flags->specifier == 'x' ||
		flags->specifier == 'p' || flags->specifier == 'X')
		u_number(flags, ap);
}

void	flush_buffer(t_flags *flags)
{
	write(flags->fd, flags->buffer, flags->buf_len);
	flags->total += flags->buf_len;
	flags->buf_len = 0;
	ft_memset(flags->buffer, 0, sizeof(flags->buffer));
}

void	add_buffer(const char *buf, t_flags *flags)
{
	if (ft_strlen(buf) + flags->buf_len > sizeof(flags->buffer) - 1)
		flush_buffer(flags);
	ft_strcat(flags->buffer, buf);
	flags->buf_len += ft_strlen(buf);
}
