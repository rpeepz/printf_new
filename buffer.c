#include "ft_printf.h"

static void	string_width(char *buffer, t_flags *flags)
{
	char	*tmp;
	int		len;

	len = strlen(buffer);
	if (len < flags->width)
	{
		tmp = malloc(flags->width + 1);
		memset(tmp, !flags->zero  || flags->specifier == 's' ?
		' ' : '0', flags->width);
		memcpy(tmp + (flags->minus ? 0 : (flags->width - len)), buffer, len);
		tmp[flags->width] = 0;
		add_buffer(tmp, flags);
		free(tmp);
	}
	else
		add_buffer(buffer, flags);
}

void		string(t_flags *flags, va_list ap)
{
	char	*v;

	v = va_arg(ap, char *);
	if (!v)
		v = "(null)";
	if (flags->precision > -1 && flags->precision < (int)strlen(v))
		v = strndup(v, flags->precision);
	else
		v = strdup(v);
	string_width(v, flags);
	free(v);
}

void		number(t_flags *flags, va_list ap)
{
	long	value;
	char	*s;
	char	*tmp;

	value = convert_length(flags->length, ap, -1);
	s = itoa_base(value, 10);
	tmp = number_precision(s, flags->precision);
	string_width(tmp ? tmp : s, flags);
	if (tmp)
		free(tmp);
	free(s);
}

void		u_number(t_flags *flags, va_list ap)
{
	unsigned long	value;
	char			*s;
	char			*tmp;

	value = convert_length(flags->length, ap, 0);
	s = utoa_base(value, flags->specifier == 'u' ? 10 : 16);
	if (flags->specifier == 'X')
		str_to_upper(s);
	if (flags->specifier == 'p')
	{
		tmp = malloc(strlen(s) + 3);
		tmp[0] = '0';
		tmp[1] = 'x';
		strcat(tmp, s);
	}
	else
		tmp = number_precision(s, flags->precision);
	string_width(tmp ? tmp : s, flags);
	if (tmp)
		free(tmp);
	free(s);
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
