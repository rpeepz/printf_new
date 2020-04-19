#include "ft_printf.h"

const static char	g_base[] = "0123456789abcdef";

char				*utoa_base(unsigned long n, int base)
{
	unsigned long	tmp;
	int				len;
	char			*s;

	tmp = n;
	len = 1;
	while (tmp /= base)
		++len;
	s = malloc(len + 1);
	memset(s, 0, len + 1);
	while (len--)
	{
		s[len] = g_base[(n % base)];
		n /= base;
	}
	return (s);
}

char				*itoa_base(long n, int base)
{
	long	tmp;
	int		len;
	char	*s;

	tmp = n < 0 ? -n : n;
	len = 1;
	while (tmp /= base)
		++len;
	len += base == 10 && n < 0 ? 1 : 0;
	s = malloc(len + 1);
	memset(s, 0, len + 1);
	tmp = n < 0 ? -n : n;
	while (len--)
	{
		s[len] = g_base[(tmp % base)];
		tmp /= base;
	}
	s[0] = base == 10 && n < 0 ? '-' : s[0];
	return (s);
}

void				str_to_upper(char *s)
{
	int		i;

	i = -1;
	while (s[++i])
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] -= 32;
	}
}

long				convert_length(int length, va_list ap, int type)
{
	unsigned long	v;

	if (length == 'h')
	{
		v = va_arg(ap, int);
		return (type ? (short)v : (unsigned short)v);
	}
	else if (length / 2 == 'h')
	{
		v = va_arg(ap, int);
		return (type ? (signed char)v : (unsigned char)v);
	}
	else if (length == 'l')
		v = type ? va_arg(ap, long) : va_arg(ap, unsigned long);
	else if (length / 2 == 'l' || length == 'z')
		v = type ? va_arg(ap, long long) : va_arg(ap, unsigned long long);
	else
		v = va_arg(ap, int);
	return (v);
}

char				*number_precision(char *buffer, int precision)
{
	char	*tmp;
	int		len;

	len = strlen(buffer);
	if (precision > len)
	{
		tmp = malloc(precision + 1);
		memset(tmp, '0', precision);
		tmp[precision] = 0;
		memcpy(tmp + (precision - len), buffer, len);
		return (tmp);
	}
	return (NULL);
}
